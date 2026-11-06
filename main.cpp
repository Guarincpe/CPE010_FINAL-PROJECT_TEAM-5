#include <iostream>
#include "hashTable.h"
#include "orderArray.h"

int main() {
    orderArray orders;   
    hashTable lookup;    

    orderDetails* o = new orderDetails;
    o->trackingNumber = "TRK001";
    o->customerName = "Test Customer";

    orders.add(o);
    lookup.insert(o);

    orderDetails* found = lookup.search("TRK001");
    if (found != nullptr)
        std::cout << "Found: " << found->customerName << "\n";
    else
        std::cout << "Not found\n";

    lookup.remove("TRK001");
    if (lookup.search("TRK001") == nullptr)
        std::cout << "Removed from table\n";
    else
        std::cout << "Still in table\n";

    return 0;
}