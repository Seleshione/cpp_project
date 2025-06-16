#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/calculator_math.h"
#include <cmath>
#include <stdexcept>

TEST_CASE("applyBinaryOperation tests") {
    CHECK(applyBinaryOperation(2, "+", 3) == 5);
    CHECK(applyBinaryOperation(5, "-", 3) == 2);
    CHECK(applyBinaryOperation(3, "*", 4) == 12);
    CHECK(applyBinaryOperation(10, "/", 2) == 5);
    CHECK(applyBinaryOperation(2, "^", 3) == 8);
    CHECK_THROWS_WITH_AS(applyBinaryOperation(2, "/", 0), "Division by zero", std::runtime_error);
    CHECK_THROWS_AS(applyBinaryOperation(2, "%", 3), std::runtime_error);
}

TEST_CASE("factorial tests") {
    CHECK(factorial(0) == 1);
    CHECK(factorial(5) == 120);
    CHECK_THROWS_AS(factorial(-1), std::runtime_error);
    CHECK_THROWS_AS(factorial(3.5), std::runtime_error);
}

TEST_CASE("convertBase tests") {
    CHECK(convertBase("10", 10, 2) == "1010");
    CHECK(convertBase("1010", 2, 10) == "10");
    CHECK(convertBase("-15", 10, 16) == "-F");
    CHECK(convertBase("FF", 16, 10) == "255");
    CHECK_THROWS_AS(convertBase("2", 2, 10), std::runtime_error);
    CHECK_THROWS_AS(convertBase("10", 1, 10), std::runtime_error);
    CHECK_THROWS_AS(convertBase("10", 10, 20), std::runtime_error);
}

TEST_CASE("applyTrigonometricOperation tests") {
    CHECK(applyTrigonometricOperation(30, "sin") == doctest::Approx(0.5));
    CHECK(applyTrigonometricOperation(180, "sin") == 0);

    CHECK(applyTrigonometricOperation(60, "cos") == doctest::Approx(0.5));
    CHECK(applyTrigonometricOperation(0, "cos") == 1);

    CHECK(applyTrigonometricOperation(45, "tan") == 1);
    CHECK_THROWS_WITH_AS(applyTrigonometricOperation(90, "tan"),
        "Tangent is not defined for this angle.", std::runtime_error);

    CHECK(applyTrigonometricOperation(45, "cot") == 1);
    CHECK_THROWS_WITH_AS(applyTrigonometricOperation(0, "cot"),
        "Cotangent is not defined for this angle.", std::runtime_error);
    CHECK_THROWS_WITH_AS(applyTrigonometricOperation(180, "cot"),
        "Cotangent is not defined for this angle.", std::runtime_error);
}
