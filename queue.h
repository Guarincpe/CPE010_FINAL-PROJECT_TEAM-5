#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

struct Order {
    int    id;
    string customerName;
    string address;
    string item;
    int    quantity;
    double price;
    string status;
};

inline int readInt(const string& prompt, int minVal = numeric_limits<int>::min(),
                   int maxVal = numeric_limits<int>::max()) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Enter a number";
        if (minVal != numeric_limits<int>::min() || maxVal != numeric_limits<int>::max())
            cout << " between " << minVal << " and " << maxVal;
        cout << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

inline string readString(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        bool blank = true;
        for (char c : s) if (!isspace(static_cast<unsigned char>(c))) { blank = false; break; }
        if (!blank) return s;
        cout << "Input cannot be empty.\n";
    }
}

inline char readChoice(const string& prompt, const string& validChars) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        if (s.size() == 1) {
            char c = static_cast<char>(tolower(static_cast<unsigned char>(s[0])));
            if (validChars.find(c) != string::npos) return c;
        }
        cout << "Invalid choice. Options: " << validChars << "\n";
    }
}

struct QueueNode {
    Order      data;
    QueueNode* next;
    QueueNode(const Order& o) : data(o), next(nullptr) {}
};

class OrderQueue {
private:
    QueueNode* head;
    QueueNode* tail;
    int        count;

public:
    OrderQueue() : head(nullptr), tail(nullptr), count(0) {}

    ~OrderQueue() {
        while (!isEmpty()) dequeue();
    }

    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;

    bool isEmpty() const { return head == nullptr; }
    int  size()    const { return count; }

    void enqueue(const Order& o) {
        QueueNode* node = new QueueNode(o);
        if (tail == nullptr) head = tail = node;
        else { tail->next = node; tail = node; }
        count++;
    }

    bool dequeue(Order* out = nullptr) {
        if (isEmpty()) return false;
        QueueNode* temp = head;
        if (out) *out = temp->data;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete temp;
        count--;
        return true;
    }

    const Order* peek() const { return head ? &head->data : nullptr; }

    const QueueNode* begin() const { return head; }

    void display() const {
        if (isEmpty()) { cout << "No pending orders.\n"; return; }
        cout << "\nPending orders (" << count << "):\n";
        for (const QueueNode* p = head; p != nullptr; p = p->next)
            cout << "  #" << p->data.id << " | " << p->data.customerName
                 << " | " << p->data.item << " x" << p->data.quantity
                 << " | P" << p->data.price << " | " << p->data.status << "\n";
    }
};

inline void addOrder(OrderQueue& queue, int& nextId) {
    cout << "\n--- Add New Order ---\n";
    Order o;
    o.id           = nextId;
    o.customerName = readString("Customer name: ");
    o.address      = readString("Delivery address: ");
    o.item         = readString("Item: ");
    o.quantity     = readInt("Quantity (1-100): ", 1, 100);
    o.price        = readInt("Total price (whole pesos): ", 1, 1000000);
    o.status       = "Pending";

    queue.enqueue(o);
    nextId++;
    cout << "Order #" << o.id << " added. Orders in queue: " << queue.size() << "\n";
}

inline void processNextOrder(OrderQueue& queue) {
    cout << "\n--- Process Next Order ---\n";
    if (queue.isEmpty()) {
        cout << "No orders to process.\n";
        return;
    }
    const Order* front = queue.peek();
    cout << "Next order: #" << front->id << " | " << front->customerName
         << " | " << front->item << " x" << front->quantity << "\n";

    if (readChoice("Mark as delivered? (y/n): ", "yn") == 'n') {
        cout << "Order left in queue.\n";
        return;
    }
    Order done;
    queue.dequeue(&done);
    done.status = "Delivered";
    cout << "Order #" << done.id << " delivered. Orders left: " << queue.size() << "\n";
}

#endif
