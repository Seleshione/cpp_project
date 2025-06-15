cmake_minimum_required(VERSION 3.10)
project(GraphicalCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_library(calculator_math src/calculator_math.cpp)
target_include_directories(calculator_math PUBLIC include)

add_executable(GraphicalCalculator src/main.cpp)
target_link_libraries(GraphicalCalculator
    sfml-graphics
    sfml-window
    sfml-system
    calculator_math
)

add_custom_command(TARGET GraphicalCalculator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.1/bin/sfml-graphics-d-2.dll"
        $<TARGET_FILE_DIR:GraphicalCalculator>
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.1/bin/sfml-window-d-2.dll"
        $<TARGET_FILE_DIR:GraphicalCalculator>
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.1/bin/sfml-system-d-2.dll"
        $<TARGET_FILE_DIR:GraphicalCalculator>
)

enable_testing()
add_subdirectory(tests)
