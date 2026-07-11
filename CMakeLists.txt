cmake_minimum_required(VERSION 4.2)
project(my_chip8_interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(my_chip8_interpreter main.cpp
        core/initializationHelper.cpp
        core/chip8.h)

target_compile_options(my_chip8_interpreter PRIVATE -Wall -Wextra -Wpedantic -Werror -Weffc++ -Wconversion -Wsign-conversion)

