#pragma once

#include <limits>
#include <string>

int readInt(const std::string& prompt, 
            int minValue = std::numeric_limits<int>::min(),
            int maxValue = std::numeric_limits<int>::max());
std::string readString(const std::string& prompt);  
char readChoice(const std::string& prompt, const std::string& validChoices);