# 🚀 Mini In-Memory Key-Value Store (C++)

A lightweight in-memory key-value store implemented in C++17.

This project is the foundation for building a high-performance, thread-safe, cache-enabled storage system.

---

## 📌 Features (Phase 1)

- In-memory storage using `std::unordered_map`
- Command-line interface (CLI)
- O(1) average time complexity for `SET`, `GET`, and `DEL`
- Modular project structure (Header + Implementation separation)

---

## 🧱 Architecture

User Input (CLI)
        ↓
Command Parser
        ↓
KVStore Class
        ↓
std::unordered_map (Storage Layer)

---

## ⚙️ Supported Commands
SET key value
GET key
DEL key
EXIT


### Example
SET name Vignesh
OK
GET name
Vignesh
DEL name
Deleted
GET name
NULL

---

## 🛠 Build & Run

### Compile
g++ -std=c++17 src/KVStore.cpp src/main.cpp -O2 -o kvstore


### Run
kvstore.exe


---

## 🧠 Concepts Demonstrated

- Hash-based storage
- Command parsing
- Modular C++ project design
- Basic system architecture layering

---

## 🔮 Upcoming Enhancements

- Capacity-limited LRU eviction
- Thread-safe access
- ThreadPool-based request handling
- Benchmarking and throughput measurement

---

## 👤 Author

Vignesh S  
Computer Science Undergraduate  
Interests: Systems Programming, Concurrency, Performance Engineering













