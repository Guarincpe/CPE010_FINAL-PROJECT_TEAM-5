#include "benchMark.h"
#include "orderArray.h"
#include "hashTable.h"
#include "search_filter.h"
#include "mergeSort.h"
#include "order.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

const int      RUNS    = 5;      
const int      LOOKUPS = 1000;   
const unsigned SEED    = 42;     

using Clock = std::chrono::steady_clock;

volatile long long sink = 0;

double elapsedMs(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

std::string pad(int value, int width) {
    std::string s = std::to_string(value);
    while (static_cast<int>(s.size()) < width) s = "0" + s;
    return s;
}

std::vector<Order> generateOrders(int n) {
    const char* areas[]    = {"Quezon City", "Manila", "Makati", "Pasig", "Taguig"};
    const char* items[]    = {"Books", "Shoes", "Keyboard", "Backpack", "Clothes"};
    const char* payments[] = {"COD", "GCash", "Maya"};

    std::mt19937 rng(SEED);
    std::vector<Order> list;
    list.reserve(n);

    for (int i = 0; i < n; i++) {
        Order o;
        o.id = i + 1;
        o.trackingNumber = "SPX" + pad(i + 1, 6);
        o.customerName = "Buyer" + std::to_string(i + 1);
        o.item = items[rng() % 5];
        o.quantity = 1 + static_cast<int>(rng() % 5);
        o.address = "Brgy. " + std::to_string(1 + rng() % 100);
        o.paymentType = payments[rng() % 3];
        o.area = areas[rng() % 5];
        o.date = "2026-11-" + pad(1 + static_cast<int>(rng() % 28), 2);
        o.time = pad(8 + static_cast<int>(rng() % 10), 2) + ":" + pad(static_cast<int>(rng() % 60), 2);
        o.status = "Pending";
        list.push_back(o);
    }
    return list;
}

} 

void runBenchmark() {
    const int sizes[] = {100, 1000, 10000, 100000};

    std::cout << "\n--- System Speed Checker ---\n";
    std::cout << "Each value is the average of " << RUNS << " runs.\n"
              << "Search columns are the total time for " << LOOKUPS << " lookups.\n\n";

    std::cout << std::left << std::setw(10) << "n"
              << std::setw(20) << "Merge sort (ms)"
              << std::setw(22) << "Linear search (ms)"
              << std::setw(22) << "Binary search (ms)"
              << "Hash lookup (ms)\n";

    for (int n : sizes) {
        std::vector<Order> base = generateOrders(n);

        double sortTotal = 0, linearTotal = 0, binaryTotal = 0, hashTotal = 0;

        for (int run = 0; run < RUNS; run++) {
            orderArray arr(n, true);
            for (const Order& o : base) arr.add(new Order(o));

            hashTable table;
            for (int i = 0; i < n; i++) table.insert(arr[i]);

            std::mt19937 pick(SEED + run);
            std::vector<std::string> timeTargets, trackingTargets;
            for (int k = 0; k < LOOKUPS; k++) {
                const Order& o = base[pick() % n];
                timeTargets.push_back(o.time);
                trackingTargets.push_back(o.trackingNumber);
            }

            auto s = Clock::now();
            mergeSort(arr, 0, n - 1, BY_TIME);
            auto e = Clock::now();
            sortTotal += elapsedMs(s, e);

            s = Clock::now();
            for (const std::string& t : timeTargets)
                sink = sink + linearSearchByKey(arr, BY_TIME, t);
            e = Clock::now();
            linearTotal += elapsedMs(s, e);

            s = Clock::now();
            for (const std::string& t : timeTargets)
                sink = sink + binarySearch(arr, BY_TIME, t);
            e = Clock::now();
            binaryTotal += elapsedMs(s, e);

            // 4. hash table lookup by tracking number
            s = Clock::now();
            for (const std::string& t : trackingTargets)
                sink = sink + (table.search(t) != nullptr ? 1 : 0);
            e = Clock::now();
            hashTotal += elapsedMs(s, e);
        }

        std::cout << std::left << std::setw(10) << n << std::fixed << std::setprecision(3)
                  << std::setw(20) << sortTotal / RUNS
                  << std::setw(22) << linearTotal / RUNS
                  << std::setw(22) << binaryTotal / RUNS
                  << hashTotal / RUNS << std::endl;
    }

    std::cout << "\nLinear and binary search look up delivery times in the list sorted by time.\n"
              << "The hash table looks up tracking numbers.\n";
}