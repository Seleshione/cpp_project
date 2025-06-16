#pragma once
#include <string>

/**
 * @brief Реализация бинарной операции
 * @param a Первый операнд
 * @param op Оператор (+, -, *, /, ^)
 * @param b Второй операнд
 * @return Результат операции
 * @throw std::runtime_error При делении на ноль или неверном операторе
 */
double applyBinaryOperation(double a, const std::string& op, double b);

/**
 * @brief Реализация вычисления факториала
 * @param x Входное число (должно быть неотрицательным целым)
 * @return Факториал числа
 * @throw std::runtime_error Для отрицательных или нецелых чисел
 */
double factorial(double x);

/**
 * @brief Реализация конвертации между системами счисления
 * @param numberStr Строковое представление числа
 * @param fromBase Исходная система счисления (2-16)
 * @param toBase Целевая система счисления (2-16)
 * @return Строковое представление числа в новой системе
 * @throw std::runtime_error При недопустимом основании системы
 * @throw std::runtime_error При некорректных цифрах числа
 */
std::string convertBase(const std::string& numberStr, int fromBase, int toBase);

/**
 * @brief Реализация тригонометрических операций
 * @param value Угол в градусах
 * @param op Идентификатор операции ("sin", "cos", "tan", "cot")
 * @return Результат вычисления
 * @throw std::runtime_error При недопустимой операции
 * @throw std::runtime_error При вычислении cot(180), tan(90), где есть деление на 0
 */
double applyTrigonometricOperation(double value, const std::string& op);
