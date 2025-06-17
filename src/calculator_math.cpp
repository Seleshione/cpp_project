#include "calculator_math.h"
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <algorithm>

/**
 * @brief Округляет число до ближайшего целого, если оно очень близко к целому.
 *
 * Функция проверяет, находится ли переданное значение в пределах малой погрешности (1e-6) от целого числа.
 * Если да - возвращает округлённое целое, иначе возвращает исходное значение.
 *
 * @param value Входное число с плавающей точкой для проверки и округления.
 * @return Округлённое целое число (если value почти целое) или исходное value.
 */
static double roundIfInteger(double value) {
    double rounded = std::round(value);
    if (std::fabs(value - rounded) < 1e-6)
        return rounded;
    return value;
}

double applyBinaryOperation(double a, const std::string& op, double b) {
    double result;
    if (op == "+")
        result = a + b;
    else if (op == "-")
        result = a - b;
    else if (op == "*")
        result = a * b;
    else if (op == "/") {
        if (std::fabs(b) < 1e-6)
            throw std::runtime_error("Division by zero");
        result = a / b;
    }
    else if (op == "^")
        result = std::pow(a, b);
    else
        throw std::runtime_error("Incorrect operator");
    return roundIfInteger(result);
}

double factorial(double x) {
    if (x < 0 || std::floor(x) != x)
        throw std::runtime_error("The factorial is defined only for non-negative integers.");
    double result = 1;
    for (int i = 1; i <= static_cast<int>(x); i++) {
        result *= i;
    }
    return roundIfInteger(result);
}

/**
 * @brief Преобразует символ в его числовое значение в системах счисления до 16-ричной
 *
 * Функция конвертирует символ цифры в соответствующее числовое значение:
 * - Цифры '0'-'9' преобразуются в значения 0-9
 * - Буквы 'A'-'F' (верхний регистр) преобразуются в значения 10-15
 * - Буквы 'a'-'f' (нижний регистр) преобразуются в значения 10-15
 *
 * @param int c Входной символ для преобразования
 * @return Числовое значение символа в системе счисления
 * @throw std::runtime_error Если символ не является корректной цифрой (0-9, A-F, a-f)
 */
static int charToVal(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    throw std::runtime_error("Incorrect digit in number");
}

std::string convertBase(const std::string& numberStr, int fromBase, int toBase) {
    if (fromBase < 2 || fromBase > 16 || toBase < 2 || toBase > 16)
        throw std::runtime_error("The base of the system should be from 2 to 16");
    bool isNegative = false;
    size_t index = 0;
    if (numberStr[0] == '-') {
        isNegative = true;
        index = 1;
    }
    long long num = 0;
    for (; index < numberStr.size(); index++) {
        int digit = charToVal(numberStr[index]);
        if (digit >= fromBase)
            throw std::runtime_error("The number does not correspond to the base of the system");
        num = num * fromBase + digit;
    }
    if (isNegative)
        num = -num;
    std::string result;
    bool neg = num < 0;
    unsigned long long n = neg ? -num : num;
    if (n == 0)
        result = "0";
    while (n > 0) {
        int remainder = n % toBase;
        char digit = (remainder < 10) ? ('0' + remainder) : ('A' + remainder - 10);
        result.push_back(digit);
        n /= toBase;
    }
    if (neg)
        result.push_back('-');
    std::reverse(result.begin(), result.end());
    return result;
}

double applyTrigonometricOperation(double value, const std::string& op) {
    double rad = value * 3.14159265358979323846 / 180.0;
    double result;
    if (op == "sin") {
        result = std::sin(rad);
    }
    else if (op == "cos") {
        result = std::cos(rad);
    }
    else if (op == "tan") {
        if (std::fabs(std::cos(rad)) < 1e-6)
            throw std::runtime_error("Tangent is not defined for this angle.");
        result = std::tan(rad);
    }
    else if (op == "cot") {
        if (std::fabs(std::sin(rad)) < 1e-6)
            throw std::runtime_error("Cotangent is not defined for this angle.");
        result = std::cos(rad) / std::sin(rad);
    }
    else {
        throw std::runtime_error("Incorrect trigonometric operation");
    }
    return roundIfInteger(result);
}
