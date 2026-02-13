#include "KVStore.h"

KVStore::KVStore(size_t cap) : capacity(cap) {}

void KVStore::set(const std::string& key, const std::string& value) {

    // If key exists → update + move to front
    if (cacheMap.find(key) != cacheMap.end()) {
        cacheList.erase(cacheMap[key]);
    }
    // If capacity reached → evict LRU
    else if (cacheList.size() >= capacity) {
        auto lru = cacheList.back();
        cacheMap.erase(lru.first);
        cacheList.pop_back();
    }

    cacheList.push_front({key, value});
    cacheMap[key] = cacheList.begin();
}

std::string KVStore::get(const std::string& key) {

    if (cacheMap.find(key) == cacheMap.end())
        return "NULL";

    auto node = cacheMap[key];
    std::string value = node->second;

    // Move accessed key to front
    cacheList.erase(node);
    cacheList.push_front({key, value});
    cacheMap[key] = cacheList.begin();

    return value;
}

bool KVStore::del(const std::string& key) {

    if (cacheMap.find(key) == cacheMap.end())
        return false;

    cacheList.erase(cacheMap[key]);
    cacheMap.erase(key);
    return true;
}

void KVStore::display() {
    for (auto &p : cacheList) {
        std::cout << "(" << p.first << ":" << p.second << ") ";
    }
    std::cout << std::endl;
}
