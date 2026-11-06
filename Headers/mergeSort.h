#pragma once

#include <vector>
#include "order.h"

bool isSmaller(const Order& a, const Order& b, bool sortByArea);
void mergeHalves(std::vector<Order>& arr, int left, int mid, int right, bool sortByArea);
void mergeSort(std::vector<Order>& arr, int left, int right, bool sortByArea);

std::vector<Order> generateRandomOrders(int numOrders);
void sortOrders(std::vector<Order>& orders);
void runBenchmark();