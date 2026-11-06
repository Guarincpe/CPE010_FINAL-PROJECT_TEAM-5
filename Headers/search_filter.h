#ifndef SEARCH_FILTER_H
#define SEARCH_FILTER_H

#include <iostream>
#include <string>

// TEMP Order struct, delete when merging and use Member 1's
struct Order {
    std::string trackingNumber;
    std::string buyerName;
    std::string item;
    int quantity;
    std::string address;
    std::string paymentType;
    std::string area;
    std::string date;   // YYYY-MM-DD
    std::string time;   // HH:MM (24 hour)
};

// what we can filter by
enum FilterKey { BY_DATE = 1, BY_AREA = 2, BY_TIME = 3 };

// gets the date, area, or time of an order
const std::string& getKey(const Order& o, FilterKey key) {
    if (key == BY_DATE) return o.date;
    if (key == BY_AREA) return o.area;
    return o.time;
}

// checks if the orders are sorted by the key
bool isSortedBy(const Order orders[], int size, FilterKey key) {
    for (int i = 1; i < size; i++) {
        if (getKey(orders[i], key) < getKey(orders[i - 1], key)) {
            return false;
        }
    }
    return true;
}

// linear search by key, returns index or -1
// best O(1), worst O(n)
int linearSearchByKey(const Order orders[], int size, FilterKey key, const std::string& value) {
    for (int i = 0; i < size; i++) {
        if (getKey(orders[i], key) == value) {
            return i;
        }
    }
    return -1;
}

// linear search by tracking number, returns index or -1
int linearSearchByTracking(const Order orders[], int size, const std::string& trackingNumber) {
    for (int i = 0; i < size; i++) {
        if (orders[i].trackingNumber == trackingNumber) {
            return i;
        }
    }
    return -1;
}

// binary search, orders must be sorted by the key
// returns index or -1
// best O(1), worst O(log n)
int binarySearch(const Order orders[], int size, FilterKey key, const std::string& value) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        const std::string& midKey = getKey(orders[mid], key);

        if (midKey == value) {
            return mid;
        } else if (midKey < value) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

// finds the first index where the key is >= value
// returns size if there is none, O(log n)
int lowerBound(const Order orders[], int size, FilterKey key, const std::string& value) {
    int low = 0;
    int high = size;

    while (low < high) {
        int mid = low + (high - low) / 2;
        if (getKey(orders[mid], key) < value) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

// finds all orders with the key from low to high
// orders must be sorted by the key
// sets startIndex to the first match and returns how many matched
// O(log n + k)
int rangeFilter(const Order orders[], int size, FilterKey key,
                const std::string& low, const std::string& high, int& startIndex) {
    startIndex = lowerBound(orders, size, key, low);

    int count = 0;
    int i = startIndex;
    while (i < size && getKey(orders[i], key) <= high) {
        count++;
        i++;
    }
    return count;
}

// prints one order in one line
void printFilteredOrder(const Order& o) {
    std::cout << o.trackingNumber << " | " << o.buyerName << " | " << o.item
              << " | " << o.area << " | " << o.date << " | " << o.time << "\n";
}

// menu option: filter orders by date, area, or time
void filterOrders(const Order orders[], int size) {
    if (size == 0) {
        std::cout << "There are no orders yet.\n";
        return;
    }

    // ask until the choice is valid
    int choice = 0;
    while (choice < 1 || choice > 3) {
        std::cout << "Filter by: 1. Date  2. Area  3. Time\nChoice: ";
        std::string line;
        if (!std::getline(std::cin, line)) return;

        if (line == "1") choice = 1;
        else if (line == "2") choice = 2;
        else if (line == "3") choice = 3;
        else std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
    }
    FilterKey key = static_cast<FilterKey>(choice);

    // binary search needs sorted orders
    if (!isSortedBy(orders, size, key)) {
        std::cout << "The orders are not sorted by this yet.\n"
                  << "Please sort them first, then try again.\n";
        return;
    }

    // leave To empty to search one value
    std::string from, to;
    std::cout << "From: ";
    std::getline(std::cin, from);
    std::cout << "To (press Enter to use the same value): ";
    std::getline(std::cin, to);
    if (to.empty()) to = from;

    int start = 0;
    int count = rangeFilter(orders, size, key, from, to, start);

    if (count == 0) {
        std::cout << "No orders found.\n";
        return;
    }

    std::cout << count << " order(s) found:\n";
    for (int i = start; i < start + count; i++) {
        printFilteredOrder(orders[i]);
    }
}

#endif