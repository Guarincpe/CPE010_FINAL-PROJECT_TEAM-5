#pragma once

#include <string>
#include "queue.h"        
#include "hashTable.h"

struct RouteStop {
    std::string trackingNumber;
    std::string address;
    RouteStop*  next;
    RouteStop(const std::string& t, const std::string& a)
        : trackingNumber(t), address(a), next(nullptr) {}
};

class DriverRoute {
private:
    RouteStop* head;
    RouteStop* tail;
    int count;

public:
    DriverRoute() : head(nullptr), tail(nullptr), count(0) {}
    ~DriverRoute();

    DriverRoute(const DriverRoute&) = delete;
    DriverRoute& operator=(const DriverRoute&) = delete;

    void addStop(const std::string& trackingNumber, const std::string& address);        
    void addUrgentStop(const std::string& trackingNumber, const std::string& address);  
    bool removeStop(const std::string& trackingNumber);                                  
    bool contains(const std::string& trackingNumber) const;                              

    void display() const;
    int  size() const { return count; }
    bool isEmpty() const { return count == 0; }
};

void addUrgentStop(DriverRoute& route, const hashTable& table);
void cancelOrder(OrderQueue& queue, DriverRoute& route, hashTable& table);
