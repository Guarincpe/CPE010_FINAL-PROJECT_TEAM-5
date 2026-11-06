#include "deliTrack.h"
#include "benchmark.h"
#include "mergeSort.h"
#include "search_filter.h"
#include "utils.h"

#include <cctype>
#include <iostream>
#include <string>

static bool isValidDate(const std::string& s) {
    if (s.size() != 10 || s[4] != '-' || s[7] != '-') return false;

    const int digitPos[] = {0, 1, 2, 3, 5, 6, 8, 9};
    for (int i : digitPos)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;

    int year  = std::stoi(s.substr(0, 4));
    int month = std::stoi(s.substr(5, 2));
    int day   = std::stoi(s.substr(8, 2));
    if (year < 1 || month < 1 || month > 12 || day < 1) return false;

    const int daysIn[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    int maxDay = daysIn[month - 1];
    if (month == 2 && leap) maxDay = 29;
    return day <= maxDay;
}

static bool isValidTime(const std::string& s) {
    if (s.size() != 5 || s[2] != ':') return false;

    const int digitPos[] = {0, 1, 3, 4};
    for (int i : digitPos)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;

    int hour   = std::stoi(s.substr(0, 2));
    int minute = std::stoi(s.substr(3, 2));
    return hour <= 23 && minute <= 59;
}

static void showOrder(const Order& o) {
    std::cout << "\n  Tracking number : " << o.trackingNumber
              << "\n  Buyer           : " << o.customerName
              << "\n  Item            : " << o.item << " x" << o.quantity
              << "\n  Address         : " << o.address
              << "\n  Payment         : " << o.paymentType
              << "\n  Area            : " << o.area
              << "\n  Scheduled       : " << o.date << " " << o.time
              << "\n  Status          : " << o.status << "\n";
}

DeliTrack::DeliTrack() : nextId(1) {}

void DeliTrack::displayMenu() const {
    std::cout << "\n===== DeliTrack =====\n"
              << "1. Add order\n"
              << "2. Process order\n"
              << "3. Search order\n"
              << "4. Sort orders\n"
              << "5. Manage route\n"
              << "6. Speed check\n"
              << "0. Exit\n";
}

void DeliTrack::run() {
    std::cout << "DeliTrack: A C++ Order Queue and Route Management\n";

    while (true) {
        displayMenu();
        int choice = readInt("Enter choice (0-6): ", 0, 6);

        if (choice == 0) {
            std::cout << "Goodbye!\n";
            return;
        }

        switch (choice) {
            case 1: addNewOrder();        break;
            case 2: processOrder();       break;
            case 3: searchMenu();         break;
            case 4: sortOrders(orders);   break;
            case 5: manageRoute();        break;
            case 6: runBenchmark();       break;
        }
    }
}

void DeliTrack::addNewOrder() {
    std::cout << "\n--- Add New Order ---\n";
    Order o;

    while (true) {
        o.trackingNumber = readString("Tracking number: ");
        if (o.trackingNumber.find(' ') != std::string::npos)
            std::cout << "The tracking number cannot contain spaces.\n";
        else if (table.search(o.trackingNumber) != nullptr)
            std::cout << "That tracking number already exists.\n";
        else
            break;
    }

    o.customerName = readString("Buyer name: ");
    o.item         = readString("Item: ");
    o.quantity     = readInt("Quantity (1-100): ", 1, 100);
    o.address      = readString("Delivery address: ");

    const char* payments[] = {"COD", "GCash", "Maya"};
    o.paymentType = payments[readInt("Payment type (1 = COD, 2 = GCash, 3 = Maya): ", 1, 3) - 1];

    o.area = readString("Delivery area (e.g. Quezon City): ");

    while (true) {
        o.date = readString("Scheduled delivery date (YYYY-MM-DD): ");
        if (isValidDate(o.date)) break;
        std::cout << "Invalid date. Use YYYY-MM-DD with a real date, for example 2026-11-05.\n";
    }
    while (true) {
        o.time = readString("Scheduled delivery time (HH:MM, 24 hour): ");
        if (isValidTime(o.time)) break;
        std::cout << "Invalid time. Use HH:MM, for example 09:30 or 14:00.\n";
    }

    o.id     = nextId++;
    o.status = "Pending";

    Order* record = new Order(o);         
    orders.add(record);
    table.insert(record);
    queue.enqueue(*record);
    route.addStop(record->trackingNumber, record->address);

    std::cout << "Order " << record->trackingNumber << " added. Orders in queue: "
              << queue.size() << "\n";
}

void DeliTrack::processOrder() {
    std::cout << "\n Process Next Order \n";
    if (queue.isEmpty()) {
        std::cout << "No orders to process.\n";
        return;
    }

    const Order* front = queue.peek();
    std::cout << "Next order: " << front->trackingNumber << " | " << front->customerName
              << " | " << front->item << " x" << front->quantity
              << " | " << front->address << "\n";

    if (readChoice("Mark as delivered? (y/n): ", "yn") == 'n') {
        std::cout << "Order left in queue.\n";
        return;
    }

    Order done;
    queue.dequeue(&done);

    Order* record = table.search(done.trackingNumber);   
    if (record != nullptr) record->status = "Delivered";
    route.removeStop(done.trackingNumber);

    std::cout << "Order " << done.trackingNumber << " delivered. Orders left in queue: "
              << queue.size() << "\n";
}

// Menu 3
void DeliTrack::searchMenu() {
    std::cout << "\n Search Order \n"
              << "1. Find by tracking number\n"
              << "2. Filter by date, area, or time\n"
              << "0. Back\n";
    int choice = readInt("Choice (0-2): ", 0, 2);

    if (choice == 1) searchByTracking();
    else if (choice == 2) filterOrders(orders);
}

// hash table lookup
void DeliTrack::searchByTracking() {
    if (orders.isEmpty()) {
        std::cout << "There are no orders yet.\n";
        return;
    }
    std::string tracking = readString("Tracking number: ");
    const Order* found = table.search(tracking);
    if (found == nullptr) {
        std::cout << "Order not found.\n";
        return;
    }
    showOrder(*found);
}

// Menu 5
void DeliTrack::manageRoute() {
    std::cout << "\n Manage Route \n"
              << "1. View route\n"
              << "2. Add urgent stop\n"
              << "3. Cancel order\n"
              << "0. Back\n";
    int choice = readInt("Choice (0-3): ", 0, 3);

    if (choice == 1) route.display();
    else if (choice == 2) addUrgentStop(route, table);
    else if (choice == 3) cancelOrder(queue, route, table);
}