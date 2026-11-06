#pragma once

#include <string>
#include "order.h"

// what we can filter by
enum FilterKey { BY_DATE = 1, BY_AREA = 2, BY_TIME = 3 };

// gives the date, area, or time of an order
const std::string& getKey(const Order& o, FilterKey key);

// checks if the orders are in order by the key
bool isSortedBy(const Order orders[], int size, FilterKey key);

// linear search by key, gives the index or -1
int linearSearchByKey(const Order orders[], int size, FilterKey key, const std::string& value);

// linear search by tracking number, gives the index or -1
int linearSearchByTracking(const Order orders[], int size, const std::string& trackingNumber);

// binary search, the orders must be sorted by the key, gives the index or -1
int binarySearch(const Order orders[], int size, FilterKey key, const std::string& value);

// finds the first order with a key equal to or bigger than the value
int lowerBound(const Order orders[], int size, FilterKey key, const std::string& value);

// finds the orders from low to high, gives how many matched and the first index in startIndex
int rangeFilter(const Order orders[], int size, FilterKey key,
                const std::string& low, const std::string& high, int& startIndex);

// prints one order in one line
void printFilteredOrder(const Order& o);

// asks what to filter by, then shows the orders that match
void filterOrders(const Order orders[], int size);