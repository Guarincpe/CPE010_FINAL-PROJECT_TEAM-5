#include "mergesort.h"
#include "utils.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstdlib>

// merge sort
bool isSmaller(const Order& a, const Order& b, bool sortByArea) {
    if (sortByArea) return a.address < b.address;
    return a.price < b.price;
}

void mergeHalves(std::vector<Order>& arr, int left, int mid, int right, bool sortByArea) {
    std::vector<Order> temp;
    int i = left, j = mid + 1;

    while (i <= mid && j <= right) {
        if (isSmaller(arr[j], arr[i], sortByArea)) temp.push_back(arr[j++]);
        else temp.push_back(arr[i++]); 
    }
    while (i <= mid)   temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int k = 0; k < (int)temp.size(); k++)
        arr[left + k] = temp[k];
}

void mergeSort(std::vector<Order>& arr, int left, int right, bool sortByArea) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid, sortByArea);
    mergeSort(arr, mid + 1, right, sortByArea);
    mergeHalves(arr, left, mid, right, sortByArea);
}

// generate test orders
std::vector<Order> generateOrders(int n) {
    std::string areas[] = {"Quezon City", "Manila", "Makati", "Pasig", "Taguig"};
    std::string items[] = {"Books", "Shoes", "Keyboard", "Backpack", "Clothes"};
    std::vector<Order> orders;
    srand(42);

    for (int i = 0; i < n; i++) {
        Order o;
        o.id           = i + 1;
        o.customerName = "Buyer" + std::to_string(i + 1);
        o.address      = areas[rand() % 5];
        o.item         = items[rand() % 5];
        o.quantity     = 1 + rand() % 5;
        o.price        = 100 + rand() % 5000;
        o.status       = "Pending";
        orders.push_back(o);
    }
    return orders;
}

// sorts orders
void sortOrders(std::vector<Order>& orders) {
    std::cout << "\n--- Sort Orders (Merge Sort) ---\n";
    if (orders.empty()) {
        std::cout << "No orders to sort.\n";
        return;
    }

    std::cout << "1. Sort by delivery area\n2. Sort by price\n";
    int choice = readInt("Choice (1-2): ", 1, 2);

    mergeSort(orders, 0, (int)orders.size() - 1, choice == 1);

    std::cout << "\nID   Customer       Address       Price\n";
    for (size_t i = 0; i < orders.size() && i < 10; i++)
        std::cout << std::left << std::setw(5)  << orders[i].id
                  << std::setw(15) << orders[i].customerName
                  << std::setw(14) << orders[i].address
                  << orders[i].price << "\n";
    if (orders.size() > 10)
        std::cout << "... and " << orders.size() - 10 << " more.\n";
}

//  cheking speed
void runBenchmark() {
    int sizes[] = {100, 1000, 10000, 100000};

    std::cout << "\nn         Sort by area (ms)   Sort by price (ms)\n";

    for (int n : sizes) {
        double totalArea = 0, totalPrice = 0;

        for (int run = 0; run < 5; run++) {   // 5 runs, then average
            std::vector<Order> a = generateOrders(n);
            std::vector<Order> b = a;

            auto s1 = std::chrono::steady_clock::now();
            mergeSort(a, 0, n - 1, true);
            auto e1 = std::chrono::steady_clock::now();

            auto s2 = std::chrono::steady_clock::now();
            mergeSort(b, 0, n - 1, false);
            auto e2 = std::chrono::steady_clock::now();

            totalArea  += std::chrono::duration<double, std::milli>(e1 - s1).count();
            totalPrice += std::chrono::duration<double, std::milli>(e2 - s2).count();
        }

        std::cout << std::left << std::setw(10) << n
                  << std::setw(20) << totalArea / 5
                  << totalPrice / 5 << "\n";
    }
}