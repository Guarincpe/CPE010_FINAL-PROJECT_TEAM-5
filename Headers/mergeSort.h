#pragma once

#include "orderArray.h"
#include "search_filter.h"   

bool isSmaller(const Order& a, const Order& b, FilterKey key);
void mergeHalves(orderArray& arr, int left, int mid, int right, FilterKey key);
void mergeSort(orderArray& arr, int left, int right, FilterKey key);

// menu action: sorts orders by date, area, or time
void sortOrders(orderArray& orders);