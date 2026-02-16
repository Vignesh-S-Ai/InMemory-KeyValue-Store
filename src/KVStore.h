#ifndef KVSTORE_H
#define KVSTORE_H

#include <unordered_map>
#include <list>
#include <string>
#include <mutex>

// ==================== LRU Shard ====================

class LRUShard {
private:
    size_t capacity;

    std::list<std::pair<std::string, std::string>> cacheList;

    std::unordered_map<
        std::string,
        std::list<std::pair<std::string, std::string>>::iterator
    > cacheMap;

    std::mutex cacheMutex;   // 🔐 Thread safety

public:
    KVStore(size_t cap);

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    void display();
};

#endif
