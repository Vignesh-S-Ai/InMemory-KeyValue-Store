🧠 Architecture (High-Level)

User Input (CLI)
↓
Command Parser
↓
Key-Value Store Core
↓
(optional) LRU Eviction
↓
Thread Safety Layer


Keep it clean like previous projects.

---

# 🥇 Phase 1: Basic KV Store (Single-Threaded)

We start simple.

No threads yet.
No LRU yet.
Just correct core logic.

---

## ✅ Step 1: Create `KVStore.h`

```cpp
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
