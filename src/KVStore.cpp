#include "KVStore.h"
#include <iostream>

KVStore::KVStore(size_t cap) : capacity(cap) {}

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
