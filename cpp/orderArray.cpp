#include "orderArray.h"

void orderArray::grow() {
    int newCapacity = capacity * 2;
    Order** newItems = new Order*[newCapacity];
    for (int i = 0; i < count; ++i) {
        newItems[i] = items[i];
    }
    delete[] items;
    items = newItems;
    capacity = newCapacity;
}

orderArray::orderArray(int initialCapacity, bool owns)
    : items(nullptr), count(0),
      capacity(initialCapacity < 1 ? 1 : initialCapacity), ownsOrders(owns) {
    items = new Order*[capacity];
}

orderArray::~orderArray() {
    if (ownsOrders) {
        for (int i = 0; i < count; ++i) {
            delete items[i];
        }
    }
    delete[] items;
}

bool orderArray::add(Order* order) {
    if (order == nullptr) return false;
    if (count == capacity) grow();
    items[count++] = order;
    return true;
}

Order* orderArray::at(int index) const {
    if (index < 0 || index >= count) return nullptr;
    return items[index];
}

void orderArray::clear() {
    if (ownsOrders) {
        for (int i = 0; i < count; ++i) {
            delete items[i];
        }
    }
    count = 0;
}
