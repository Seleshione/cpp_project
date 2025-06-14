cmake_minimum_required(VERSION 3.10)
project(GraphicalCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь к SFML
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Добавляем библиотеку с математическими функциями
add_library(calculator_math src/calculator_math.cpp)
target_include_directories(calculator_math PUBLIC include)

# Основное приложение
add_executable(GraphicalCalculator src/main.cpp)
target_link_libraries(GraphicalCalculator
    sfml-graphics
    sfml-window
    sfml-system
    calculator_math  # Добавляем нашу библиотеку
)

# Копирование DLL файлов SFML
add_custom_command(TARGET GraphicalCalculator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "C:/SFML-2.6.1/bin/sfml-graphics-d-2.dll"
        $<TARGET_FILE_DIR:GraphicalCalculator>
    COMMAND ${CMAKE_COMMAND} -E copy
        "C:/SFML-2.6.1/bin/sfml-window-d-2.dll"
        $<TARGET_FILE_DIR:GraphicalCalculator>
    COMMAND ${CMAKE_COMMAND} -E copy
        "C:/SFML-2.6.1/bin/sfml-system-d-2.dll"
        $<TARGET_FILE_DIR:GraphicalCalculator>
)

# Включаем тестирование
enable_testing()
add_subdirectory(tests)