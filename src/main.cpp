#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <bitset>
#include "calculator_math.h"

/**
 * @brief Структура, представляющая графическую кнопку калькулятора
 *
 * Содержит визуальные элементы (прямоугольник и текст) и функциональную метку.
 */
struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::string label;
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

    sf::FloatRect textRect = btn.text.getLocalBounds();
    btn.text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    btn.text.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
    return btn;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphical Calculator", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    std::string fontPath = "Sansation_Bold.ttf";
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font from: " << fontPath << std::endl;
        return EXIT_FAILURE;
    }

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
    std::string calcDisplay = "0";

    double storedValue = 0.0;
    std::string pendingOp = "";
    bool awaitingBinaryOperation = false;
    bool resetDisplay = false;
    bool isConverting = false;
    bool isEnteringBase = false;
    int targetBase = 10;
    std::string numberToConvert = "";
    std::string baseInput = "";

    std::vector<Button> buttons;

    sf::Vector2f basicStart(10, 100);
    sf::Vector2f basicSize(90, 70);
    float basicSpacing = 10;

    std::vector<std::vector<std::string>> basicLabels = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", ".", "=", "+"}
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sf::Vector2f pos(basicStart.x + j * (basicSize.x + basicSpacing),
                basicStart.y + i * (basicSize.y + basicSpacing));
            buttons.push_back(createButton(basicLabels[i][j], font, pos, basicSize));
        }
    }

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

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++) {
            sf::Vector2f pos(funcStart.x + j * (funcSize.x + funcSpacing),
                funcStart.y + i * (funcSize.y + funcSpacing));
            buttons.push_back(createButton(funcLabels[i][j], font, pos, funcSize));
        }
    }

    /**
    * @brief Обновляет текстовый дисплей калькулятора
    *
    * @param calcDisplay Текущее значение для отображения (строка)
    * @param displayText SFML-текст для визуализации
    * @param displayRect SFML-прямоугольник, определяющий границы дисплея
    */
    auto updateDisplayText = [&]() {
        displayText.setString(calcDisplay);
        };
    updateDisplayText();

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
                                    if (isConverting) {
                                        if (isEnteringBase) {
                                            if (!baseInput.empty()) {
                                                baseInput.pop_back();
                                                if (baseInput.empty()) {
                                                    isEnteringBase = false;
                                                    calcDisplay = "Base? (2-16)";
                                                }
                                                else {
                                                    calcDisplay = "Base: " + baseInput;
                                                }
                                            }
                                        }
                                        else {
                                            isConverting = false;
                                            calcDisplay = numberToConvert;
                                        }
                                    }
                                    else {
                                        if (!calcDisplay.empty()) {
                                            calcDisplay.pop_back();
                                            if (calcDisplay.empty())
                                                calcDisplay = "0";
                                        }
                                    }
                                }
                                else if (label == "conv") {
                                    if (!isConverting) {
                                        numberToConvert = calcDisplay;
                                        isConverting = true;
                                        isEnteringBase = true;
                                        baseInput = "";
                                        calcDisplay = "Base? (2-16)";
                                    }
                                }
                                else if (isdigit(label[0])) {
                                    if (isConverting && isEnteringBase) {
                                        baseInput += label;
                                        calcDisplay = "Base: " + baseInput;

                                        if (baseInput.length() == 2) { 
                                            targetBase = std::stoi(baseInput);
                                            if (targetBase >= 2 && targetBase <= 16) {
                                                calcDisplay = convertBase(numberToConvert, 10, targetBase);
                                                isConverting = false;
                                                isEnteringBase = false;
                                            }
                                            else {
                                                calcDisplay = "Invalid base";
                                                isConverting = false;
                                                isEnteringBase = false;
                                            }
                                        }
                                    }
                                    else if (!isConverting) {
                                        if (resetDisplay) {
                                            calcDisplay = "";
                                            resetDisplay = false;
                                        }
                                        if (calcDisplay == "0")
                                            calcDisplay = label;
                                        else
                                            calcDisplay += label;
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
                                        throw std::runtime_error("cot of 0");
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
                                else if (label == ".") {
                                    if (calcDisplay.find('.') == std::string::npos) {
                                        calcDisplay += ".";
                                    }
                                }
                            }
                            catch (std::exception& ex) {
                                calcDisplay = std::string("Error: ") + ex.what();
                                awaitingBinaryOperation = false;
                                pendingOp = "";
                                resetDisplay = true;
                                isConverting = false;
                                isEnteringBase = false;
                            }
                            updateDisplayText();
                            break;
                        }
                    }
                }
            }
        }

        if (isConverting) {
            displayRect.setFillColor(sf::Color(200, 230, 200));
        }
        else {
            displayRect.setFillColor(sf::Color(200, 200, 200));
        }

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
