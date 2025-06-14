/**
 * @file graphical_calculator.cpp
 * @brief Реализация графического калькулятора с использованием SFML
 */

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Структура, представляющая графическую кнопку калькулятора
 * 
 * Содержит визуальные элементы (прямоугольник и текст) и функциональную метку.
 */
struct Button {
    sf::RectangleShape shape; ///< Графическая форма кнопки
    sf::Text text;           ///< Текст на кнопке
    std::string label;       ///< Функциональная метка кнопки
};

/**
 * @brief Создает кнопку с заданными параметрами
 * 
 * @param label Текстовая метка кнопки
 * @param font Шрифт для текста кнопки
 * @param pos Позиция кнопки (верхний левый угол)
 * @param size Размер кнопки
 * @return Button Готовая кнопка с настроенными параметрами
 */
Button createButton(const std::string& label, const sf::Font& font, sf::Vector2f pos, sf::Vector2f size) {
    Button btn;
    btn.label = label;
    btn.shape.setPosition(pos);
    btn.shape.setSize(size);
    btn.shape.setFillColor(sf::Color::White);
    btn.shape.setOutlineColor(sf::Color::Black);
    btn.shape.setOutlineThickness(2.f);

    btn.text.setFont(font);
    btn.text.setString(label);
    btn.text.setCharacterSize(20);
    btn.text.setFillColor(sf::Color::Black);
    
    // Центрирование текста в кнопке
    sf::FloatRect textRect = btn.text.getLocalBounds();
    btn.text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    btn.text.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
    return btn;
}

/**
 * @brief Основная функция программы
 * 
 * Создает графический интерфейс калькулятора и обрабатывает пользовательский ввод.
 * 
 * @return int Код завершения программы
 */
int main() {
    // Создание окна 800x600 пикселей
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphical Calculator", sf::Style::Titlebar | sf::Style::Close);

    // Загрузка шрифта
    sf::Font font;
    std::string fontPath = "C:/Users/notebook/Desktop/shr/Sansation_Bold.ttf";
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font from: " << fontPath << std::endl;
        return EXIT_FAILURE;
    }

    // Создание дисплея калькулятора
    sf::RectangleShape displayRect(sf::Vector2f(780, 80));
    displayRect.setFillColor(sf::Color(200, 200, 200));
    displayRect.setPosition(10, 10);
    displayRect.setOutlineColor(sf::Color::Black);
    displayRect.setOutlineThickness(2.f);

    sf::Text displayText;
    displayText.setFont(font);
    displayText.setCharacterSize(24);
    displayText.setFillColor(sf::Color::Black);
    displayText.setPosition(20, 20);
    std::string calcDisplay = "0"; // Текущее значение на дисплее

    // Список всех кнопок калькулятора
    std::vector<Button> buttons;

    // --- Основные кнопки (4x4) ---
    sf::Vector2f basicStart(10, 100);
    sf::Vector2f basicSize(90, 70);
    float basicSpacing = 10;

    std::vector<std::vector<std::string>> basicLabels = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", ".", "=", "+"}
    };

    // Создание основных кнопок
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sf::Vector2f pos(basicStart.x + j * (basicSize.x + basicSpacing),
                basicStart.y + i * (basicSize.y + basicSpacing));
            Button b = createButton(basicLabels[i][j], font, pos, basicSize);
            buttons.push_back(b);
        }
    }

    // --- Функциональные кнопки (2x5) ---
    sf::Vector2f funcStart(420, 100);
    sf::Vector2f funcSize(150, 60);
    float funcSpacing = 10;

    std::vector<std::vector<std::string>> funcLabels = {
        {"sin", "cos"},
        {"tan", "cot"},
        {"^", "!"},
        {"+/-", "conv"},
        {"DEL", "EXIT"}
    };

    // Создание функциональных кнопок
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++) {
            sf::Vector2f pos(funcStart.x + j * (funcSize.x + funcSpacing),
                funcStart.y + i * (funcSize.y + funcSpacing));
            Button b = createButton(funcLabels[i][j], font, pos, funcSize);
            buttons.push_back(b);
        }
    }

    // Состояние калькулятора
    double storedValue = 0.0;      ///< Хранимое значение для операций
    std::string pendingOp = "";    ///< Ожидающая операция
    bool awaitingBinaryOperation = false; ///< Флаг ожидания бинарной операции
    bool resetDisplay = false;     ///< Флаг сброса дисплея

    // Лямбда для обновления текста на дисплее
    auto updateDisplayText = [&]() {
        displayText.setString(calcDisplay);
    };
    updateDisplayText();

    /**
     * @brief Применяет бинарную операцию к двум числам
     * 
     * @param a Первый операнд
     * @param op Оператор (+, -, *, /, ^)
     * @param b Второй операнд
     * @return double Результат операции
     * @throws std::runtime_error При делении на ноль или неверном операторе
     */
    auto applyBinaryOperation = [&](double a, const std::string& op, double b) -> double {
        if (op == "+") return a + b;
        else if (op == "-") return a - b;
        else if (op == "*") return a * b;
        else if (op == "/") {
            if (b == 0) throw std::runtime_error("Деление на ноль");
            return a / b;
        }
        else if (op == "^") return std::pow(a, b);
        else throw std::runtime_error("Неизвестная операция");
    };

    /**
     * @brief Вычисляет факториал числа
     * 
     * @param x Входное число (должно быть неотрицательным целым)
     * @return double Факториал числа
     * @throws std::runtime_error Для отрицательных или нецелых чисел
     */
    auto factorial = [&](double x) -> double {
        if (x < 0 || std::floor(x) != x)
            throw std::runtime_error("Факториал определен только для натуральных чисел");
        double res = 1;
        for (int i = 1; i <= static_cast<int>(x); i++) {
            res *= i;
        }
        return res;
    };

    /**
     * @brief Конвертирует число в другую систему счисления
     * 
     * @param number Число для конвертации
     * @param base Целевая система счисления (2-16)
     * @return std::string Строковое представление числа в новой системе
     */
    auto convertBase = [&](long long number, int base) -> std::string {
        if (number == 0) return "0";
        bool neg = false;
        if (number < 0) {
            neg = true;
            number = -number;
        }
        std::string digits = "0123456789ABCDEF";
        std::string result;
        while (number > 0) {
            int rem = number % base;
            result.push_back(digits[rem]);
            number /= base;
        }
        if (neg)
            result.push_back('-');
        std::reverse(result.begin(), result.end());
        return result;
    };

    /**
     * @brief Выполняет конвертацию числа и отображает результаты
     */
    auto performConversion = [&]() {
        try {
            double val = std::stod(calcDisplay);
            long long intVal = static_cast<long long>(val);
            std::ostringstream oss;
            std::vector<int> bases = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 16 };
            for (int b : bases) {
                oss << b << ": " << convertBase(intVal, b) << "\n";
            }
            calcDisplay = oss.str();
            resetDisplay = true;
        }
        catch (...) {
            calcDisplay = "Ошибка конвертации";
            resetDisplay = true;
        }
        updateDisplayText();
    };

    // Главный цикл программы
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                    for (auto& btn : buttons) {
                        if (btn.shape.getGlobalBounds().contains(mousePos)) {
                            std::string label = btn.label;
                            try {
                                if (label == "EXIT") {
                                    window.close();
                                }
                                else if (label == "DEL") {
                                    if (!calcDisplay.empty()) {
                                        calcDisplay.pop_back();
                                        if (calcDisplay.empty())
                                            calcDisplay = "0";
                                    }
                                }
                                else if (label == "+" || label == "-" || label == "*" || label == "/" || label == "^") {
                                    double currentVal = std::stod(calcDisplay);
                                    if (!awaitingBinaryOperation) {
                                        storedValue = currentVal;
                                        pendingOp = label;
                                        awaitingBinaryOperation = true;
                                        resetDisplay = true;
                                    }
                                    else {
                                        double result = applyBinaryOperation(storedValue, pendingOp, currentVal);
                                        storedValue = result;
                                        pendingOp = label;
                                        calcDisplay = std::to_string(result);
                                        resetDisplay = true;
                                    }
                                }
                                else if (label == "=") {
                                    if (awaitingBinaryOperation) {
                                        double currentVal = std::stod(calcDisplay);
                                        double result = applyBinaryOperation(storedValue, pendingOp, currentVal);
                                        calcDisplay = std::to_string(result);
                                        awaitingBinaryOperation = false;
                                        pendingOp = "";
                                        resetDisplay = true;
                                    }
                                }
                                else if (label == "sin") {
                                    double val = std::stod(calcDisplay);
                                    double result = std::sin(val);
                                    calcDisplay = std::to_string(result);
                                    resetDisplay = true;
                                }
                                else if (label == "cos") {
                                    double val = std::stod(calcDisplay);
                                    double result = std::cos(val);
                                    calcDisplay = std::to_string(result);
                                    resetDisplay = true;
                                }
                                else if (label == "tan") {
                                    double val = std::stod(calcDisplay);
                                    double result = std::tan(val);
                                    calcDisplay = std::to_string(result);
                                    resetDisplay = true;
                                }
                                else if (label == "cot") {
                                    double val = std::stod(calcDisplay);
                                    double tanVal = std::tan(val);
                                    if (tanVal == 0)
                                        throw std::runtime_error("������� �� 0 � cot");
                                    double result = 1.0 / tanVal;
                                    calcDisplay = std::to_string(result);
                                    resetDisplay = true;
                                }
                                else if (label == "!") {
                                    double val = std::stod(calcDisplay);
                                    double result = factorial(val);
                                    calcDisplay = std::to_string(result);
                                    resetDisplay = true;
                                }
                                else if (label == "+/-") {
                                    if (!calcDisplay.empty() && calcDisplay[0] == '-')
                                        calcDisplay.erase(0, 1);
                                    else
                                        calcDisplay = "-" + calcDisplay;
                                }
                                else if (label == "conv") {
                                    performConversion();
                                }
                                else {
                                    if (resetDisplay) {
                                        calcDisplay = "";
                                        resetDisplay = false;
                                    }
                                    if (calcDisplay == "0" && label != ".")
                                        calcDisplay = label;
                                    else
                                        calcDisplay += label;
                                }
                            }
                            catch (std::exception& ex) {
                                calcDisplay = std::string("������: ") + ex.what();
                                awaitingBinaryOperation = false;
                                pendingOp = "";
                                resetDisplay = true;
                            }
                            updateDisplayText();
                            break;  
                        }
                    }
                }
            }
        }
        // Отрисовка интерфейса
        window.clear(sf::Color::White);
        window.draw(displayRect);
        window.draw(displayText);
        for (auto& btn : buttons) {
            window.draw(btn.shape);
            window.draw(btn.text);
        }
        window.display();
    }
    return 0;
}