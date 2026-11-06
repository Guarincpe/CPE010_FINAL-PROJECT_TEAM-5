#include "queue.h"

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
        std::cout << "  " << p->data.trackingNumber << " | " << p->data.customerName
                  << " | " << p->data.item << " x" << p->data.quantity
                  << " | " << p->data.status << "\n";
}