cmake_minimum_required(VERSION 4.2)
project(my_chip8_interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(my_chip8_interpreter main.cpp
        core/initializationHelper.cpp
        core/initializationHelper.h
        core/chip8.cpp
        core/chip8.h
        platform/window.cpp
        platform/window.h
        platform/polling.cpp)

target_compile_options(my_chip8_interpreter PRIVATE -Wall -Wextra -Wpedantic -Werror -Weffc++ -Wconversion -Wsign-conversion)

find_package(SDL3 REQUIRED CONFIG)

target_link_libraries(my_chip8_interpreter PRIVATE SDL3::SDL3)