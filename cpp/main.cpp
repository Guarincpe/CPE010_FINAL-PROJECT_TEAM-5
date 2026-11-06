#include <iostream>
#include <string>
#include "search_filter.h"

int main() {
    // sample orders, sorted by date and time
    // area is not sorted on purpose, to test the sorted check
    const int SIZE = 8;
    Order orders[SIZE] = {
        {"SPX001", "Juan Dela Cruz",   "Barong Tagalog", 1, "Brgy. Commonwealth, Quezon City", "COD",   "Quezon City", "2026-11-01", "08:00"},
        {"SPX002", "Maria Santos",     "Kapeng Barako",  3, "Brgy. Concepcion, Marikina",      "GCash", "Marikina",    "2026-11-01", "09:00"},
        {"SPX003", "Jose Reyes",       "Dried Mangoes",  2, "Brgy. Kapitolyo, Pasig",          "COD",   "Pasig",       "2026-11-02", "09:00"},
        {"SPX004", "Angel Bautista",   "Ube Halaya",     5, "Brgy. Poblacion, Makati",         "Maya",  "Makati",      "2026-11-02", "10:30"},
        {"SPX005", "Mark Villanueva",  "Banig Mat",      1, "Brgy. Wack-Wack, Mandaluyong",    "COD",   "Mandaluyong", "2026-11-03", "11:00"},
        {"SPX006", "Liza Mendoza",     "Tsinelas",       4, "Brgy. Fort Bonifacio, Taguig",    "GCash", "Taguig",      "2026-11-03", "13:00"},
        {"SPX007", "Rico Dizon",       "Bayong Bag",     2, "Brgy. Pinagkaisahan, Quezon City","COD",   "Quezon City", "2026-11-04", "14:30"},
        {"SPX008", "Cheska Ramos",     "Walis Tambo",    1, "Brgy. Santolan, Pasig",           "Maya",  "Pasig",       "2026-11-04", "16:00"}
    };

    std::string choice;

    while (true) {
        std::cout << "\nDeliTrack - Search Test\n";
        std::cout << "1. Filter orders\n";
        std::cout << "2. Linear search (tracking number)\n";
        std::cout << "3. Binary search (time)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        if (!std::getline(std::cin, choice)) break;

        if (choice == "0") {
            std::cout << "Goodbye!\n";
            break;
        } else if (choice == "1") {
            filterOrders(orders, SIZE);
        } else if (choice == "2") {
            std::string tracking;
            std::cout << "Tracking number: ";
            std::getline(std::cin, tracking);

            int index = linearSearchByTracking(orders, SIZE, tracking);
            if (index == -1) {
                std::cout << "Order not found.\n";
            } else {
                printFilteredOrder(orders[index]);
            }
        } else if (choice == "3") {
            std::string time;
            std::cout << "Time (HH:MM): ";
            std::getline(std::cin, time);

            int index = binarySearch(orders, SIZE, BY_TIME, time);
            if (index == -1) {
                std::cout << "Order not found.\n";
            } else {
                printFilteredOrder(orders[index]);
            }
        } else {
            std::cout << "Invalid choice. Please enter 0 to 3.\n";
        }
    }

    return 0;
}