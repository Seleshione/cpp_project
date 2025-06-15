#include "calculator_math.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <cctype>

double applyBinaryOperation(double a, const std::string& op, double b) {
    if (op == "+") {
        return a + b;
    } else if (op == "-") {
        return a - b;
    } else if (op == "*") {
        return a * b;
    } else if (op == "/") {
        if (b == 0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    } else if (op == "^") {
        return std::pow(a, b);
    } else {
        throw std::runtime_error("Unknown operator");
    }
}

double factorial(double x) {
    if (x < 0 || std::floor(x) != x) {
        throw std::runtime_error("Factorial is only defined for non-negative integers");
    }
    
    double result = 1;
    for (int i = 1; i <= static_cast<int>(x); ++i) {
        result *= i;
    }
    return result;
}

std::string convertBase(const std::string& numberStr, int fromBase, int toBase) {
    if (fromBase < 2 || fromBase > 16 || toBase < 2 || toBase > 16) {
        throw std::runtime_error("Base must be between 2 and 16");
    }

    if (numberStr.empty()) {
        throw std::runtime_error("Empty number string");
    }

    const std::string validDigits = "0123456789ABCDEF";
    bool isNegative = false;
    size_t startPos = 0;

    if (numberStr[0] == '-') {
        isNegative = true;
        startPos = 1;
    }

    for (size_t i = startPos; i < numberStr.size(); ++i) {
        char c = toupper(numberStr[i]);
        size_t pos = validDigits.find(c);
        if (pos == std::string::npos || pos >= static_cast<size_t>(fromBase)) {
            throw std::runtime_error("Invalid digit for the given base");
        }
    }

    long long decimalValue = 0;
    for (size_t i = startPos; i < numberStr.size(); ++i) {
        char c = toupper(numberStr[i]);
        int digit = (c >= 'A') ? (10 + c - 'A') : (c - '0');
        decimalValue = decimalValue * fromBase + digit;
    }

    if (isNegative) {
        decimalValue = -decimalValue;
    }

    if (decimalValue == 0) {
        return "0";
    }

    std::string result;
    long long number = decimalValue;
    if (number < 0) {
        isNegative = true;
        number = -number;
    } else {
        isNegative = false;
    }

    while (number > 0) {
        int remainder = number % toBase;
        result.push_back(validDigits[remainder]);
        number /= toBase;
    }

    if (isNegative) {
        result.push_back('-');
    }

    std::reverse(result.begin(), result.end());
    return result;
}

double applyTrigonometricOperation(double value, const std::string& op) {
    if (op == "sin") {
        return std::sin(value);
    } else if (op == "cos") {
        return std::cos(value);
    } else if (op == "tan") {
        return std::tan(value);
    } else if (op == "cot") {
        double tanVal = std::tan(value);
        if (tanVal == 0) {
            throw std::runtime_error("Division by zero in cotangent");
        }
        return 1.0 / tanVal;
    } else {
        throw std::runtime_error("Unknown trigonometric operation");
    }
}
