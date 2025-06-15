cmake_minimum_required(VERSION 3.10)

file(GLOB TEST_SOURCES *.cpp)

add_executable(GraphicalCalculatorTests ${TEST_SOURCES})

target_link_libraries(GraphicalCalculatorTests
    PRIVATE calculator_math
)

target_include_directories(GraphicalCalculatorTests
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/doctest
)

add_test(NAME GraphicalCalculatorTests COMMAND GraphicalCalculatorTests)
