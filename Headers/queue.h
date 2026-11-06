#pragma once

#include "order.h"

struct QueueNode {
    Order      data;
    QueueNode* next;
    QueueNode(const Order& o) : data(o), next(nullptr) {}
};

// FIFO queue (linked list) of pending orders.
class OrderQueue {
private:
    QueueNode* head;
    QueueNode* tail;
    int        count;

public:
    OrderQueue() : head(nullptr), tail(nullptr), count(0) {}
    ~OrderQueue();

    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;

    bool isEmpty() const { return head == nullptr; }
    int  size() const { return count; }

    void enqueue(const Order& o);
    bool dequeue(Order* out = nullptr);

    const Order* peek()  const { return head ? &head->data : nullptr; }
    const QueueNode* begin() const { return head; }

    void display() const;
};