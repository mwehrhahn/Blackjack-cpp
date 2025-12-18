#include "Input.h"
#include <iostream>
#include <limits>

int readIntInRange(const std::string& prompt, int min, int max) {
    while (true) {
        std::cout << prompt;
        int value;

        if (std::cin >> value && value >= min && value <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Invalid input.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDoubleMin(const std::string& prompt, double min) {
    while (true) {
        std::cout << prompt;
        double value;

        if (std::cin >> value && value >= min) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Invalid input.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}
