#include "KVStore.h"
#include <iostream>
#include <sstream>

int main() {
    KVStore kv;
    std::string line;

    std::cout << "Mini In-Memory Key-Value Store\n";
    std::cout << "Commands: SET key value | GET key | DEL key | EXIT\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        std::stringstream ss(line);
        std::string command, key, value;

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
        else if (command == "EXIT") {
            break;
        }
        else {
            std::cout << "Invalid command\n";
        }
    }

    return 0;
}
