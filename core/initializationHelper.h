//
// Created by amrit on 11/07/26.
//

#ifndef MY_CHIP8_INTERPRETER_CHIP8_H
#define MY_CHIP8_INTERPRETER_CHIP8_H

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// max gamefile size limited to 3.5 KiB
constexpr uintmax_t MAX_GAMEFILE_SIZE{3584};

bool isPathValid(const fs::path& romPath);

#endif //MY_CHIP8_INTERPRETER_CHIP8_H
