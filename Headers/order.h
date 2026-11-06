#pragma once

#include <string>

struct Order {
    std::string trackingNumber;
    std::string customerName;      
    std::string item;
    int quantity = 0;
    std::string address;
    std::string paymentType;
    std::string area;
    std::string date;              
    std::string time;              

    int id    = 0;
    double price = 0.0;
    std::string status = "Pending";
};
