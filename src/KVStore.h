#ifndef KVSTORE_H
#define KVSTORE_H

#include <unordered_map>
#include <string>

class KVStore {
private:
    std::unordered_map<std::string, std::string> store;

public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
};

#endif
