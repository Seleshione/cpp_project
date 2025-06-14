cmake_minimum_required(VERSION 3.10)

file(GLOB TEST_SOURCES *.cpp)

# Создаем тестовый исполняемый файл
add_executable(GraphicalCalculatorTests ${TEST_SOURCES})

# Подключаем нашу библиотеку и заголовки
target_link_libraries(GraphicalCalculatorTests
    PRIVATE calculator_math
)

# Указываем пути к заголовочным файлам
target_include_directories(GraphicalCalculatorTests
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/doctest
)

# Регистрируем тест для CTest
add_test(NAME GraphicalCalculatorTests COMMAND GraphicalCalculatorTests)