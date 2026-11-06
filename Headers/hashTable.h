#pragma once

#include <string>
#include "orderDetails.h"

class hashTable {
private:
    struct Node {
        orderDetails* order;
        Node* next;
        Node(orderDetails* order, Node* n) : order(order), next(n) {}
    };

    Node** table;
    int tableSize;
    int count;

    static constexpr double loadFactorThreshold = 0.75;

    static unsigned long rawHash(const std::string& key);
    int  indexFor(const std::string& key) const;
    void reHash();
    void freeAllNodes();

public:
    explicit hashTable(int initialSize = 101);
    ~hashTable();

    hashTable(const hashTable&) = delete;
    hashTable& operator=(const hashTable&) = delete;

    bool          insert(orderDetails* order);
    orderDetails* search(const std::string& trackingNumber) const;
    bool          remove(const std::string& trackingNumber);

    int    getCount()     const { return count; }
    int    getTableSize() const { return tableSize; }
    double loadFactor()   const { return static_cast<double>(count) / tableSize; }
    bool   isEmpty()      const { return count == 0; }
};
