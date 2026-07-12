//
// Created by amrit on 11/07/26.
//

#ifndef MY_CHIP8_INTERPRETER_INITIALIZATION_HELPER_H
#define MY_CHIP8_INTERPRETER_INITIALIZATION_HELPER_H

#include "chip8.h"

namespace fs = std::filesystem;

// max gamefile size limited to 3.5 KiB
inline constexpr uintmax_t MAX_GAMEFILE_SIZE{3584};

bool isPathValid(const fs::path& romPath);
Chip8 initiateChip8(std::filesystem::path& gamePath);
void initFont(Chip8& cpu);
void loadGameFile(std::filesystem::path& gamePath, Chip8& chip8);

#endif //MY_CHIP8_INTERPRETER_INITIALIZATION_HELPER_H
