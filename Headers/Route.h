#pragma once

#include <string>
#include "queue.h"        // OrderQueue, Order, readString, readChoice
#include "hashTable.h"

// one stop on the driver's route (singly linked list node).
struct RouteStop {
    std::string trackingNumber;
    std::string address;
    RouteStop*  next;
    RouteStop(const std::string& t, const std::string& a)
        : trackingNumber(t), address(a), next(nullptr) {}
};

// driver route kept as a linked list so stops can be added or removed
// mid-route without rebuilding anything.
class DriverRoute {
private:
    RouteStop* head;
    RouteStop* tail;
    int        count;

public:
    DriverRoute() : head(nullptr), tail(nullptr), count(0) {}
    ~DriverRoute();

    DriverRoute(const DriverRoute&) = delete;
    DriverRoute& operator=(const DriverRoute&) = delete;

    void addStop(const std::string& trackingNumber, const std::string& address);        // end of route, O(1)
    void addUrgentStop(const std::string& trackingNumber, const std::string& address);  // front of route, O(1)
    bool removeStop(const std::string& trackingNumber);                                  // O(n)
    bool contains(const std::string& trackingNumber) const;                              // O(n)

    void display() const;
    int  size()    const { return count; }
    bool isEmpty() const { return count == 0; }
};

void addUrgentStop(DriverRoute& route, const hashTable& table);
void cancelOrder(OrderQueue& queue, DriverRoute& route, hashTable& table);
