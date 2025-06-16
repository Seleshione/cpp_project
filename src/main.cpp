#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "calculator_math.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

/**
 * @brief Структура, представляющая графическую кнопку калькулятора
 * Содержит визуальные элементы (прямоугольник и текст) и функциональную метку.
 */
struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::string label;
};

/**
* @brief Главная функция приложения калькулятора
* Инициализирует графический интерфейс, обрабатывает пользовательский ввод и выполняет математические операции.
* @return int Код завершения программы (0 - успешное выполнение)
*/
int main() {
    const int windowWidth = 500;
    const int windowHeight = 700;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Calculator");

    sf::Font font;
    if (!font.loadFromFile("Sansation_Bold.ttf")) {
        std::cerr << "Ошибка загрузки шрифта Sansation_Bold.ttf" << std::endl;
        return -1;
    }

    sf::Text display;
    display.setFont(font);
    display.setCharacterSize(24);
    display.setFillColor(sf::Color::White);
    display.setPosition(10, 10);

    std::string expression;
    std::vector<std::string> buttonLabels = {
        "7", "8", "9", "+", "-",
        "4", "5", "6", "*", "/",
        "1", "2", "3", "x!", "C",
        "0", ".", "=", "sin", "cos",
        "tan", "cot",
        "2-cc", "3-cc", "4-cc", "5-cc", "6-cc", "7-cc", "8-cc", "9-cc", "16-cc",
        "±"
    };

    std::vector<Button> buttons;
    const int margin = 10;   
    const int displayHeight = 50;
    const int gridTop = displayHeight + 20; 
    const int gridHeight = windowHeight - gridTop - margin;

    const int cols = 5;
    int rows = (buttonLabels.size() + cols - 1) / cols;  

    float btnWidth = (windowWidth - (cols + 1) * margin) / static_cast<float>(cols);
    float btnHeight = (gridHeight - (rows + 1) * margin) / static_cast<float>(rows);

    for (size_t i = 0; i < buttonLabels.size(); i++) {
        int row = i / cols;
        int col = i % cols;
        float posX = margin + col * (btnWidth + margin);
        float posY = gridTop + margin + row * (btnHeight + margin);

        Button btn;
        btn.label = buttonLabels[i];
        btn.shape.setSize(sf::Vector2f(btnWidth, btnHeight));
        btn.shape.setFillColor(sf::Color(100, 100, 100));
        btn.shape.setPosition(posX, posY);

        btn.text.setFont(font);
        btn.text.setString(btn.label);
        btn.text.setCharacterSize(20);
        btn.text.setFillColor(sf::Color::White);

        sf::FloatRect textRect = btn.text.getLocalBounds();
        btn.text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        btn.text.setPosition(posX + btnWidth / 2.0f, posY + btnHeight / 2.0f);

        buttons.push_back(btn);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    for (auto& btn : buttons) {
                        if (btn.shape.getGlobalBounds().contains(mousePos)) {
                            std::string key = btn.label;
                            if (expression == "Error" && key != "C") {
                                expression = "";
                            }
                            if (key == "C") {
                                expression = "";
                            }
                            else if (key == "=") {
                                size_t opPos = expression.find_first_of("+-*/^");
                                if (opPos != std::string::npos) {
                                    std::string left = expression.substr(0, opPos);
                                    std::string right = expression.substr(opPos + 1);
                                    char op = expression[opPos];
                                    try {
                                        double a = std::stod(left);
                                        double b = std::stod(right);
                                        double result = applyBinaryOperation(a, std::string(1, op), b);
                                        expression = std::to_string(result);
                                    }
                                    catch (const std::exception& ex) {
                                        expression = "Error";
                                    }
                                }
                            }
                            else if (key == "x!") {
                                try {
                                    double val = std::stod(expression);
                                    double res = factorial(val);
                                    expression = std::to_string(res);
                                }
                                catch (const std::exception& ex) {
                                    expression = "Error";
                                }
                            }
                            else if (key == "sin" || key == "cos" ||
                                key == "tan" || key == "cot") {
                                try {
                                    double angle = std::stod(expression);
                                    double res = applyTrigonometricOperation(angle, key);
                                    expression = std::to_string(res);
                                }
                                catch (const std::exception& ex) {
                                    expression = "Error";
                                }
                            }
                            else if (key.find("-cc") != std::string::npos) {
                                try {
                                    size_t pos = key.find("-cc");
                                    int targetBase = std::stoi(key.substr(0, pos));
                                    expression = convertBase(expression, 10, targetBase);
                                }
                                catch (const std::exception& ex) {
                                    expression = "Error";
                                }
                            }
                            else if (key == "±") {
                                if (expression.empty()) {
                                    expression = "-";
                                }
                                else {
                                    if (expression[0] == '-') {
                                        expression = expression.substr(1);
                                    }
                                    else {
                                        expression = "-" + expression;
                                    }
                                }
                            }
                            else if (key == "+" || key == "-" || key == "*" || key == "/" || key == "^") {
                                std::string operators = "+-*/^";
                                if (expression.empty() ||
                                    operators.find(expression.back()) != std::string::npos) {
                                    expression = "Error";
                                }
                                else {
                                    expression += key;
                                }
                            }
                            else {
                                expression += key;
                            }
                            display.setString(expression);
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(display);
        for (auto& btn : buttons) {
            window.draw(btn.shape);
            window.draw(btn.text);
        }
        window.display();
    }
    return 0;
}
