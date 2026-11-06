#include "hashTable.h"

unsigned long hashTable::rawHash(const std::string& key) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = hash * 33 + static_cast<unsigned char>(c);
    }
    return hash;
}

int hashTable::indexFor(const std::string& key) const {
    return static_cast<int>(rawHash(key) % static_cast<unsigned long>(tableSize));
}

void hashTable::reHash() {
    int newSize = tableSize * 2 + 1;
    Node** newTable = new Node*[newSize]();

    for (int i = 0; i < tableSize; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* next = current->next;
            int newIndex = static_cast<int>(
                rawHash(current->order->trackingNumber) % static_cast<unsigned long>(newSize));
            current->next = newTable[newIndex];
            newTable[newIndex] = current;
            current = next;
        }
    }
    delete[] table;
    table = newTable;
    tableSize = newSize;
}

void hashTable::freeAllNodes() {
    for (int i = 0; i < tableSize; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    count = 0;
}

hashTable::hashTable(int initialSize)
    : table(nullptr), tableSize(initialSize < 1 ? 1 : initialSize), count(0) {
    table = new Node*[tableSize]();
}

hashTable::~hashTable() {
    freeAllNodes();
    delete[] table;
}

bool hashTable::insert(orderDetails* order) {
    if (order == nullptr)
        return false;

    if (search(order->trackingNumber) != nullptr)
        return false;

    if (static_cast<double>(count + 1) / tableSize > loadFactorThreshold) {
        reHash();
    }

    int index = indexFor(order->trackingNumber);
    table[index] = new Node(order, table[index]);
    count++;
    return true;
}

orderDetails* hashTable::search(const std::string& trackingNumber) const {
    Node* current = table[indexFor(trackingNumber)];
    while (current != nullptr) {
        if (current->order->trackingNumber == trackingNumber)
            return current->order;
        current = current->next;
    }
    return nullptr;
}

bool hashTable::remove(const std::string& trackingNumber) {
    Node** link = &table[indexFor(trackingNumber)];
    while (*link != nullptr) {
        if ((*link)->order->trackingNumber == trackingNumber) {
            Node* toDelete = *link;
            *link = toDelete->next;
            delete toDelete;
            count--;
            return true;
        }
        link = &((*link)->next);
    }
    return false;
}
