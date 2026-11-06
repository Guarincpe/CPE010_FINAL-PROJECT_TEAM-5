#pragma once

#include "orderDetails.h"

class orderArray {
private:
    orderDetails** items;
    int  count;
    int  capacity;
    bool ownsOrders;

    void grow();

public:
    explicit orderArray(int initialCapacity = 16, bool owns = true);
    ~orderArray();

    orderArray(const orderArray&) = delete;
    orderArray& operator=(const orderArray&) = delete;

    bool          add(orderDetails* order);
    orderDetails* at(int index) const;      // checked, nullptr if out of range
    void          clear();

    // Unchecked access for sorting and searching (keep 0 <= index < size).
    orderDetails*& operator[](int index)       { return items[index]; }
    orderDetails*  operator[](int index) const { return items[index]; }

    int  size()        const { return count; }
    int  getCapacity() const { return capacity; }
    bool isEmpty()     const { return count == 0; }
};
