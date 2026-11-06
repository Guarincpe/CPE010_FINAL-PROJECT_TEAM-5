#include "mergeSort.h"
#include "utils.h"

#include <iomanip>
#include <iostream>
#include <vector>

// merge sort
bool isSmaller(const Order& a, const Order& b, FilterKey key) {
    return getKey(a, key) < getKey(b, key);
}

void mergeHalves(orderArray& arr, int left, int mid, int right, FilterKey key) {
    std::vector<Order*> temp;
    temp.reserve(right - left + 1);
    int i = left, j = mid + 1;

    while (i <= mid && j <= right) {
        if (isSmaller(*arr[j], *arr[i], key)) temp.push_back(arr[j++]);
        else                                  temp.push_back(arr[i++]); 
    }
    while (i <= mid)   temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int k = 0; k < (int)temp.size(); k++)
        arr[left + k] = temp[k];
}

void mergeSort(orderArray& arr, int left, int right, FilterKey key) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid, key);
    mergeSort(arr, mid + 1, right, key);
    mergeHalves(arr, left, mid, right, key);
}

// sorts orders
void sortOrders(orderArray& orders) {
    std::cout << "\n--- Sort Orders (Merge Sort) ---\n";
    if (orders.isEmpty()) {
        std::cout << "No orders to sort.\n";
        return;
    }

    std::cout << "1. Sort by delivery date\n2. Sort by delivery area\n3. Sort by delivery time\n";
    int choice = readInt("Choice (1-3): ", 1, 3);

    mergeSort(orders, 0, orders.size() - 1, static_cast<FilterKey>(choice));

    std::cout << "\nTracking   Customer       Area          Date        Time\n";
    for (int i = 0; i < orders.size() && i < 10; i++) {
        const Order& o = *orders[i];
        std::cout << std::left << std::setw(11) << o.trackingNumber
                  << std::setw(15) << o.customerName
                  << std::setw(14) << o.area
                  << std::setw(12) << o.date
                  << o.time << "\n";
    }
    if (orders.size() > 10)
        std::cout << "... and " << orders.size() - 10 << " more.\n";
}