#pragma once

#include <string>
#include "orderArray.h"

// what we can filter by
enum FilterKey { BY_DATE = 1, BY_AREA = 2, BY_TIME = 3 };

// gets the date, area, or time of an order
const std::string& getKey(const Order& o, FilterKey key);

// checks if the orders are sorted by the key
bool isSortedBy(const orderArray& orders, FilterKey key);

// linear search by key, returns index or -1
// best O(1), worst O(n)
int linearSearchByKey(const orderArray& orders, FilterKey key, const std::string& value);

// linear search by tracking number, returns index or -1
int linearSearchByTracking(const orderArray& orders, const std::string& trackingNumber);

// binary search, orders must be sorted by the key
// returns index or -1
// best O(1), worst O(log n)
int binarySearch(const orderArray& orders, FilterKey key, const std::string& value);

// finds the first index where the key is >= value
// returns size if there is none, O(log n)
int lowerBound(const orderArray& orders, FilterKey key, const std::string& value);

// finds all orders with the key from low to high
// orders must be sorted by the key
// sets startIndex to the first match and returns how many matched
// O(log n + k)
int rangeFilter(const orderArray& orders, FilterKey key,
                const std::string& low, const std::string& high, int& startIndex);

// prints one order in one line
void printFilteredOrder(const Order& o);

// menu option: filter orders by date, area, or time
void filterOrders(const orderArray& orders);