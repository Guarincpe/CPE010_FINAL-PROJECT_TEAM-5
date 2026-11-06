#pragma once

#include <string>

struct Order{
    int id;
    std::string customerName;
    std::string address;
    std::string item;
    int quantity;
    double price;
    std::string status;
};