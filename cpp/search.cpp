#include "search_filter.h"

#include <iostream>
#include <string>

// gives the date, area, or time of an order
const std::string& getKey(const Order& o, FilterKey key) {
    if (key == BY_DATE) return o.date;
    if (key == BY_AREA) return o.area;
    return o.time;
}

// checks if the orders are in order by the key
bool isSortedBy(const Order orders[], int size, FilterKey key) {
    for (int i = 1; i < size; i++) {
        if (getKey(orders[i], key) < getKey(orders[i - 1], key)) {
            return false;
        }
    }
    return true;
}

// checks the orders one by one, gives the index or -1
int linearSearchByKey(const Order orders[], int size, FilterKey key, const std::string& value) {
    for (int i = 0; i < size; i++) {
        if (getKey(orders[i], key) == value) {
            return i;
        }
    }
    return -1;
}

// same as above but uses the tracking number
int linearSearchByTracking(const Order orders[], int size, const std::string& trackingNumber) {
    for (int i = 0; i < size; i++) {
        if (orders[i].trackingNumber == trackingNumber) {
            return i;
        }
    }
    return -1;
}

// cuts the sorted list in half each time, gives the index or -1
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

// finds the first order with a key equal to or bigger than the value
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

// finds the first match, then goes forward until the key is past high
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
    std::cout << o.trackingNumber << " | " << o.customerName << " | " << o.item
              << " | " << o.area << " | " << o.date << " | " << o.time << "\n";
}

// asks what to filter by, then shows the orders that match
void filterOrders(const Order orders[], int size) {
    if (size == 0) {
        std::cout << "There are no orders yet.\n";
        return;
    }

    // keep asking until the choice is valid
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

    // binary search only works on sorted orders
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

    // show the matches
    std::cout << count << " order(s) found:\n";
    for (int i = start; i < start + count; i++) {
        printFilteredOrder(orders[i]);
    }
}