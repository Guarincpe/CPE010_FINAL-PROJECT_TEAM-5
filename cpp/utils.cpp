#include "utils.h"

#include <iostream>
#include <cctype>

int readInt(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Invalid input. Enter a number";
        if (minVal != std::numeric_limits<int>::min() || maxVal != std::numeric_limits<int>::max())
            std::cout << " between " << minVal << " and " << maxVal;
        std::cout << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readString(const std::string& prompt) {
    std::string s;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, s);
        bool blank = true;
        for (char c : s) if (!std::isspace(static_cast<unsigned char>(c))) { blank = false; break; }
        if (!blank) return s;
        std::cout << "Input cannot be empty.\n";
    }
}

char readChoice(const std::string& prompt, const std::string& validChars) {
    std::string s;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, s);
        if (s.size() == 1) {
            char c = static_cast<char>(std::tolower(static_cast<unsigned char>(s[0])));
            if (validChars.find(c) != std::string::npos) return c;
        }
        std::cout << "Invalid choice. Options: " << validChars << "\n";
    }
}