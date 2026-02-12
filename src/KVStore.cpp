#include "KVStore.h"

void KVStore::set(const std::string& key, const std::string& value) {
    store[key] = value;
}

std::string KVStore::get(const std::string& key) {
    if (store.find(key) == store.end()) {
        return "NULL";
    }
    return store[key];
}

bool KVStore::del(const std::string& key) {
    return store.erase(key) > 0;
}
