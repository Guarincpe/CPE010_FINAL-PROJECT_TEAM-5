#include "queue.h"
#include "utils.h"

#include <iostream>

OrderQueue::~OrderQueue() {
    while (!isEmpty()) dequeue();
}

void OrderQueue::enqueue(const Order& o) {
    QueueNode* node = new QueueNode(o);
    if (tail == nullptr) head = tail = node;
    else { tail->next = node; tail = node; }
    count++;
}

bool OrderQueue::dequeue(Order* out) {
    if (isEmpty()) return false;
    QueueNode* temp = head;
    if (out) *out = temp->data;
    head = head->next;
    if (head == nullptr) tail = nullptr;
    delete temp;
    count--;
    return true;
}

void OrderQueue::display() const {
    if (isEmpty()) { std::cout << "No pending orders.\n"; return; }
    std::cout << "\nPending orders (" << count << "):\n";
    for (const QueueNode* p = head; p != nullptr; p = p->next)
        std::cout << "  #" << p->data.id << " | " << p->data.customerName
                  << " | " << p->data.item << " x" << p->data.quantity
                  << " | P" << p->data.price << " | " << p->data.status << "\n";
}

void addOrder(OrderQueue& queue, int& nextId) {
    std::cout << "\n Add New Order \n";
    Order o;
    o.id = nextId;
    o.customerName = readString("Customer name: ");
    o.address = readString("Delivery address: ");
    o.item = readString("Item: ");
    o.quantity = readInt("Quantity (1-100): ", 1, 100);
    o.price = readInt("Total price (whole pesos): ", 1, 1000000);
    o.status = "Pending";

    queue.enqueue(o);
    nextId++;
    std::cout << "Order #" << o.id << " added. Orders in queue: " << queue.size() << "\n";
}

void processNextOrder(OrderQueue& queue) {
    std::cout << "\n--- Process Next Order ---\n";
    if (queue.isEmpty()) {
        std::cout << "No orders to process.\n";
        return;
    }
    const Order* front = queue.peek();
    std::cout << "Next order: #" << front->id << " | " << front->customerName
              << " | " << front->item << " x" << front->quantity << "\n";

    if (readChoice("Mark as delivered? (y/n): ", "yn") == 'n') {
        std::cout << "Order left in queue.\n";
        return;
    }
    Order done;
    queue.dequeue(&done);
    done.status = "Delivered";
    std::cout << "Order #" << done.id << " delivered. Orders left: " << queue.size() << "\n";
}