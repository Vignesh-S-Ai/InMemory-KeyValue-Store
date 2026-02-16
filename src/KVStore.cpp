#include "KVStore.h"
#include <iostream>

KVStore::KVStore(size_t cap) : capacity(cap) {}

// ==================== LRUShard ====================

LRUShard::LRUShard(size_t cap) : capacity(cap) {

    cacheMap.reserve(capacity);

    head = new Node{"", "", nullptr, nullptr};
    tail = new Node{"", "", nullptr, nullptr};

    head->next = tail;
    tail->prev = head;
}

LRUShard::~LRUShard() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void LRUShard::addToFront(Node* node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

void LRUShard::removeNode(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void LRUShard::set(const std::string& key, const std::string& value) {

    std::lock_guard<std::mutex> lock(cacheMutex);
    metrics.sets++;

    auto it = cacheMap.find(key);

    if (it != cacheMap.end()) {
        Node* node = it->second;
        node->value = value;
        removeNode(node);
        addToFront(node);
        return;
    }

    if (currentSize >= capacity) {
        Node* lru = tail->prev;
        cacheMap.erase(lru->key);
        removeNode(lru);
        delete lru;
        currentSize--;
        metrics.evictions++;
    }

    Node* newNode = new Node{key, value, nullptr, nullptr};
    addToFront(newNode);
    cacheMap[key] = newNode;
    currentSize++;
}

std::string LRUShard::get(const std::string& key) {

    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = cacheMap.find(key);

    if (it == cacheMap.end()) {
        metrics.misses++;
        return "NULL";
    }

    metrics.hits++;

    Node* node = it->second;
    removeNode(node);
    addToFront(node);

    return node->value;
}

bool LRUShard::del(const std::string& key) {

    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = cacheMap.find(key);
    if (it == cacheMap.end())
        return false;

    Node* node = it->second;
    removeNode(node);
    cacheMap.erase(key);
    delete node;
    currentSize--;
    metrics.dels++;

    return true;
}

void LRUShard::getMetrics(size_t& hits, size_t& misses,
                          size_t& sets, size_t& dels,
                          size_t& evictions) const {

    hits = metrics.hits.load();
    misses = metrics.misses.load();
    sets = metrics.sets.load();
    dels = metrics.dels.load();
    evictions = metrics.evictions.load();
}

// ==================== Sharded KVStore ====================

KVStore::KVStore(size_t totalCapacity, size_t shardCount)
    : numShards(shardCount)
{
    size_t perShardCapacity = totalCapacity / shardCount;

    for (size_t i = 0; i < shardCount; ++i) {
        shards.push_back(std::make_unique<LRUShard>(perShardCapacity));
    }
}

void KVStore::set(const std::string& key, const std::string& value) {

    std::lock_guard<std::mutex> lock(cacheMutex);

    if (cacheMap.find(key) != cacheMap.end()) {
        cacheList.erase(cacheMap[key]);
    }
    else if (cacheList.size() >= capacity) {
        auto lru = cacheList.back();
        cacheMap.erase(lru.first);
        cacheList.pop_back();
    }

    cacheList.push_front({key, value});
    cacheMap[key] = cacheList.begin();
}

std::string KVStore::get(const std::string& key) {

    std::lock_guard<std::mutex> lock(cacheMutex);

    if (cacheMap.find(key) == cacheMap.end())
        return "NULL";

    auto node = cacheMap[key];
    std::string value = node->second;

    cacheList.erase(node);
    cacheList.push_front({key, value});
    cacheMap[key] = cacheList.begin();

    return value;
}

bool KVStore::del(const std::string& key) {

    std::lock_guard<std::mutex> lock(cacheMutex);

    if (cacheMap.find(key) == cacheMap.end())
        return false;

    cacheList.erase(cacheMap[key]);
    cacheMap.erase(key);
    return true;
}

void KVStore::display() {

    std::lock_guard<std::mutex> lock(cacheMutex);

    std::cout << "Cache State (MRU → LRU): ";
    for (auto &p : cacheList) {
        std::cout << "(" << p.first << ":" << p.second << ") ";
    }
    std::cout << "\n";
}
