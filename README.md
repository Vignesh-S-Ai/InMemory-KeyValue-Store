# 🚀 Mini In-Memory Key-Value Store (C++)

A high-performance, concurrent, sharded LRU key-value store implemented in **C++17**.

This project demonstrates systems-level design, memory layout optimization, and multi-threaded performance benchmarking.

---

## 📌 Overview

This is a custom-built in-memory key-value store supporting:

- O(1) `SET`, `GET`, and `DEL`
- LRU (Least Recently Used) eviction
- Sharded architecture for scalability
- Thread-safe operations
- Custom doubly-linked node implementation
- Built-in metrics & benchmark reporting

The system was optimized through multiple architectural improvements and validated with real performance measurements.

---

## 🧠 Architecture

### 🔹 Sharded Design

Instead of a single global lock, the cache is divided into multiple shards:

Key → Hash → Shard → Local LRU + Mutex

Each shard maintains:

- Independent hash map
- Independent LRU structure
- Dedicated mutex
- Per-shard metrics

This reduces lock contention and improves scalability under concurrent workloads.

---

### 🔹 Custom LRU Core

The initial implementation used `std::list`.

It was replaced with a custom doubly-linked node structure:

```cpp
struct Node {
    std::string key;
    std::string value;
    Node* prev;
    Node* next;
};


This significantly improved:

Memory locality

CPU cache utilization

Allocation overhead

Overall throughput

⚙️ Features
Phase 1

Basic in-memory key-value store

CLI interaction

O(1) operations

Thread safety with mutex

Phase 2

Sharded concurrency model

Multi-threaded benchmark

Phase 3

Metrics system:

Hits

Misses

Evictions

Sets

Deletes

Phase 4

Custom LRU node optimization

2× throughput improvement

📊 Benchmark Results

Machine:

6 Physical Cores

12 Logical Processors

Before Optimization (std::list)
Threads	Throughput
6	~2.46M ops/sec
12	~2.65M ops/sec
After Custom Node Optimization
Threads	Throughput
6	5.12M ops/sec
12	5.81M ops/sec

🚀 Achieved ~2× performance improvement by optimizing memory layout.

Sample Metrics Output

Threads: 12
Total Operations: 2,400,000

Hit Rate: ~97%

Evictions: ~18,000

Balanced shard distribution

No global lock bottleneck

🔧 Build Instructions
Compile
g++ -std=c++17 src/KVStore.cpp src/main.cpp -pthread -O2 -o kvbench

Run Benchmark
./kvbench

📈 What This Project Demonstrates

Concurrent system design

Lock contention reduction via sharding

Memory layout optimization

Hardware-aware performance tuning

Observability using metrics

Empirical benchmarking methodology

🎯 Key Learnings

Memory layout can impact performance more than locking strategy.

Sharding reduces contention significantly.

std::list can become a bottleneck in high-performance systems.

Hardware limits become visible after architectural bottlenecks are removed.

Always measure before optimizing.

🏁 Version

v1.0 – Stable Release

Includes:

Sharded LRU architecture

Custom node optimization

Metrics system

Performance benchmarking

🚀 Future Improvements

TCP-based cache server

TTL expiration support

Lock contention timing instrumentation

Distributed consistent hashing

Persistent storage layer

##👨‍💻 Author

Vignesh
C++ Systems & Performance Engineering
---
