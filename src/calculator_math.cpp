#include "calculator_math.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <sstream>

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

std::string convertBase(long long number, int base) {
    if (base < 2 || base > 16) {
        throw std::runtime_error("Base must be between 2 and 16");
    }

    if (number == 0) {
        return "0";
    }

    bool isNegative = false;
    if (number < 0) {
        isNegative = true;
        number = -number;
    }

    const std::string digits = "0123456789ABCDEF";
    std::string result;

    while (number > 0) {
        int remainder = number % base;
        result.push_back(digits[remainder]);
        number /= base;
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