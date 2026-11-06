#pragma once

#include <string>

struct orderDetails{
    
    std::string trackingNumber;
    std::string customerName;
    std::string customerAddress;
    std::string customerPhone;
    std::string orderDate;
    std::string deliveryTime;
    std::string item;
    std::string paymentType;
    std::string status = "Pending";

    int quantity = 0;
};