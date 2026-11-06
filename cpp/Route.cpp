#include "Route.h"
#include "utils.h"
#include <iostream>

// DriverRoute

DriverRoute::~DriverRoute() {
    while (head != nullptr) {
        RouteStop* next = head->next;
        delete head;
        head = next;
    }
}

void DriverRoute::addStop(const std::string& trackingNumber, const std::string& address) {
    RouteStop* stop = new RouteStop(trackingNumber, address);
    if (tail == nullptr) head = tail = stop;
    else { tail->next = stop; tail = stop; }
    count++;
}

void DriverRoute::addUrgentStop(const std::string& trackingNumber, const std::string& address) {
    RouteStop* stop = new RouteStop(trackingNumber, address);
    stop->next = head;
    head = stop;
    if (tail == nullptr) tail = stop;
    count++;
}

bool DriverRoute::removeStop(const std::string& trackingNumber) {
    RouteStop* prev = nullptr;
    RouteStop* cur  = head;
    while (cur != nullptr) {
        if (cur->trackingNumber == trackingNumber) {
            if (prev == nullptr) head = cur->next;
            else prev->next = cur->next;
            if (cur == tail) tail = prev;
            delete cur;
            count--;
            return true;
        }
        prev = cur;
        cur  = cur->next;
    }
    return false;
}

bool DriverRoute::contains(const std::string& trackingNumber) const {
    for (const RouteStop* p = head; p != nullptr; p = p->next)
        if (p->trackingNumber == trackingNumber) return true;
    return false;
}

void DriverRoute::display() const {
    if (isEmpty()) {
        std::cout << "The route is empty.\n";
        return;
    }
    std::cout << "\nDriver route (" << count << " stops):\n";
    int n = 1;
    for (const RouteStop* p = head; p != nullptr; p = p->next, n++)
        std::cout << "  " << n << ". " << p->trackingNumber << " | " << p->address << "\n";
}

// ---------- Menu functions ----------

// Moves an existing order to the front of the route (or puts it there if it
// is not on the route yet).
void addUrgentStop(DriverRoute& route, const hashTable& table) {
    std::cout << "\n--- Add Urgent Stop ---\n";
    std::string tracking = readString("Tracking number: ");

    Order* order = table.search(tracking);
    if (order == nullptr) {
        std::cout << "Order not found.\n";
        return;
    }
    if (order->status != "Pending") {
        std::cout << "Order is already " << order->status << ". It cannot be added to the route.\n";
        return;
    }

    bool wasOnRoute = route.removeStop(tracking);
    route.addUrgentStop(tracking, order->address);

    std::cout << tracking << (wasOnRoute ? " moved to" : " added to")
              << " the front of the route.\n";
}

// Rebuilds the queue without the order that has the given id.
// Uses only enqueue/dequeue so the arrival order of the rest is kept.
static void removeFromQueue(OrderQueue& queue, int id) {
    int n = queue.size();
    for (int i = 0; i < n; i++) {
        Order o;
        queue.dequeue(&o);
        if (o.id != id) queue.enqueue(o);
    }
}

// cancels a pending order: removes it from the queue and the route and
// marks it "Cancelled". The record stays in the hash table so it can
// still be tracked.
void cancelOrder(OrderQueue& queue, DriverRoute& route, hashTable& table) {
    std::cout << "\n--- Cancel Order ---\n";
    std::string tracking = readString("Tracking number: ");

    Order* order = table.search(tracking);
    if (order == nullptr) {
        std::cout << "Order not found.\n";
        return;
    }
    if (order->status != "Pending") {
        std::cout << "Order is already " << order->status << ". It cannot be cancelled.\n";
        return;
    }

    std::cout << "Cancel order " << tracking << " (" << order->customerName
              << ", " << order->item << ")?\n";
    if (readChoice("Confirm (y/n): ", "yn") == 'n') {
        std::cout << "Order kept.\n";
        return;
    }

    removeFromQueue(queue, order->id);
    route.removeStop(tracking);
    order->status = "Cancelled";

    std::cout << "Order " << tracking << " cancelled. Orders in queue: " << queue.size()
              << ", stops on route: " << route.size() << "\n";
}
