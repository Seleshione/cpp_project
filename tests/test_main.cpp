#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/calculator_math.h"
#include <cmath>
#include <stdexcept>

TEST_CASE("Binary operations") {
    SUBCASE("Positive cases") {
        CHECK(applyBinaryOperation(2, "+", 3) == 5);
        CHECK(applyBinaryOperation(5, "-", 3) == 2);
        CHECK(applyBinaryOperation(2, "*", 3) == 6);
        CHECK(applyBinaryOperation(6, "/", 3) == doctest::Approx(2.0));
        CHECK(applyBinaryOperation(2, "^", 3) == 8);
    }

    SUBCASE("Negative cases") {
        CHECK_THROWS_AS(applyBinaryOperation(5, "/", 0), std::runtime_error);
        CHECK_THROWS_AS(applyBinaryOperation(5, "?", 3), std::runtime_error);
    }
}

TEST_CASE("Factorial") {
    SUBCASE("Positive cases") {
        CHECK(factorial(0) == 1);
        CHECK(factorial(1) == 1);
        CHECK(factorial(5) == 120);
    }

    SUBCASE("Negative cases") {
        CHECK_THROWS_AS(factorial(-1), std::runtime_error);
        CHECK_THROWS_AS(factorial(3.5), std::runtime_error);
    }
}

TEST_CASE("Base conversion") {
    SUBCASE("Positive cases") {
        CHECK(convertBase(10, 2) == "1010");
        CHECK(convertBase(10, 16) == "A");
        CHECK(convertBase(-5, 2) == "-101");
        CHECK(convertBase(0, 2) == "0");
    }

    SUBCASE("Negative cases") {
        CHECK_THROWS_AS(convertBase(10, 1), std::runtime_error);
        CHECK_THROWS_AS(convertBase(10, 17), std::runtime_error);
    }
}

TEST_CASE("Trigonometric functions") {
    const double PI = 3.14159265358979323846;
    const double epsilon = 1e-6;

    SUBCASE("Sine function") {
        CHECK(std::sin(0.0) == doctest::Approx(0.0).epsilon(epsilon));
        CHECK(std::sin(PI / 2) == doctest::Approx(1.0).epsilon(epsilon));
        CHECK(std::sin(PI) == doctest::Approx(0.0).epsilon(epsilon));
        CHECK(std::sin(3 * PI / 2) == doctest::Approx(-1.0).epsilon(epsilon));
    }

    SUBCASE("Cosine function") {
        CHECK(std::cos(0.0) == doctest::Approx(1.0).epsilon(epsilon));
        CHECK(std::cos(PI / 2) == doctest::Approx(0.0).epsilon(epsilon));
        CHECK(std::cos(PI) == doctest::Approx(-1.0).epsilon(epsilon));
        CHECK(std::cos(3 * PI / 2) == doctest::Approx(0.0).epsilon(epsilon));
    }

    SUBCASE("Tangent function") {
        CHECK(std::tan(0.0) == doctest::Approx(0.0).epsilon(epsilon));
        CHECK(std::tan(PI / 4) == doctest::Approx(1.0).epsilon(epsilon));
        CHECK(std::tan(-PI / 4) == doctest::Approx(-1.0).epsilon(epsilon));
    }

    SUBCASE("Cotangent function") {
        SUBCASE("Normal cases") {
            CHECK(1.0 / std::tan(PI / 4) == doctest::Approx(1.0).epsilon(epsilon));
            CHECK(1.0 / std::tan(3 * PI / 4) == doctest::Approx(-1.0).epsilon(epsilon));
        }

        SUBCASE("Edge cases") {

            double cot0 = 1.0 / std::tan(0.0);
            CHECK(!std::isfinite(cot0));

            CHECK(1.0 / std::tan(PI / 2) == doctest::Approx(0.0).epsilon(epsilon));
        }
    }
}

TEST_CASE("Trigonometric functions in calculator") {
    const double PI = 3.14159265358979323846;
    const double epsilon = 1e-6;

    SUBCASE("Sine") {
        CHECK(applyTrigonometricOperation(0.0, "sin") == doctest::Approx(0.0).epsilon(epsilon));
        CHECK(applyTrigonometricOperation(PI / 2, "sin") == doctest::Approx(1.0).epsilon(epsilon));
    }

    SUBCASE("Cosine") {
        CHECK(applyTrigonometricOperation(0.0, "cos") == doctest::Approx(1.0).epsilon(epsilon));
        CHECK(applyTrigonometricOperation(PI, "cos") == doctest::Approx(-1.0).epsilon(epsilon));
    }

    SUBCASE("Tangent") {
        CHECK(applyTrigonometricOperation(0.0, "tan") == doctest::Approx(0.0).epsilon(epsilon));
        CHECK(applyTrigonometricOperation(PI / 4, "tan") == doctest::Approx(1.0).epsilon(epsilon));
    }

    SUBCASE("Cotangent") {
        CHECK(applyTrigonometricOperation(PI / 4, "cot") == doctest::Approx(1.0).epsilon(epsilon));
        CHECK_THROWS_AS(applyTrigonometricOperation(0.0, "cot"), std::runtime_error);
    }

    SUBCASE("Invalid operation") {
        CHECK_THROWS_AS(applyTrigonometricOperation(0.0, "invalid"), std::runtime_error);
    }
}