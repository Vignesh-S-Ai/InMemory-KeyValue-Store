#include "KVStore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>

int main() {

    KVStore kv(3);   // Set capacity here

    std::string line;

    std::cout << "Mini In-Memory Key-Value Store (LRU Enabled)\n";
    std::cout << "Commands: SET key value | GET key | DEL key | SHOW | EXIT\n";

    for (int i = 0; i < operations; ++i) {

        int key = keyDist(gen);
        int op = opDist(gen);

        ss >> command;

        if (command == "SET") {
            ss >> key >> value;
            kv.set(key, value);
            std::cout << "OK\n";
        }
        else if (command == "GET") {
            ss >> key;
            std::cout << kv.get(key) << "\n";
        }
        else if (command == "DEL") {
            ss >> key;
            if (kv.del(key))
                std::cout << "Deleted\n";
            else
                std::cout << "Key not found\n";
        }
        else if (command == "SHOW") {
            kv.display();
        }
        else if (command == "EXIT") {
            break;
        }
        else {
            std::cout << "Invalid command\n";
        }
    }
}

int main() {

    const int THREADS = 12;
    const int OPERATIONS_PER_THREAD = 200000;
    const int KEY_RANGE = 1000;   // Increased for better shard distribution

    KVStore kv(1000, 8);   // 50 capacity, 4 shards

    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(worker, std::ref(kv),
                             OPERATIONS_PER_THREAD, KEY_RANGE);
    }

    for (auto& t : threads)
        t.join();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    long totalOps = THREADS * OPERATIONS_PER_THREAD;

    std::cout << "\n=== KV Store Concurrency Benchmark ===\n";
    std::cout << "Threads           : " << THREADS << "\n";
    std::cout << "Total Operations  : " << totalOps << "\n";
    std::cout << "Execution Time    : " << duration.count() << " ms\n";
    std::cout << "Throughput        : "
              << (totalOps * 1000.0 / duration.count())
              << " ops/sec\n";

    kv.printStats();

    return 0;
}
