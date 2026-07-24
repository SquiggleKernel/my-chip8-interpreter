//
// Created by amrit on 11/07/26.
//

#include "initializationHelper.h"
#include "chip8.h"


/* check if path points to regular file and it is less than 3.5 KiB
 * if it is not print to std error and return false
 */
bool isPathValid(const fs::path& romPath) {
    if (!fs::exists(romPath)) {
        std::cerr<< "file does not exist\n";
        return false;
    }
    else if (!fs::is_regular_file(romPath)) {
        std::cerr << "not a file\n";
        return false;
    }
    if (fs::file_size(romPath)> MAX_GAMEFILE_SIZE) {
        std::cerr << "invalid rom [size too big]\n";
        return false;
    }
    return true;
}


/* take in gamefile path
 * outputs a Chip8 struct
 * initialized all components of chip8 interpreter
 */
Chip8 initiateChip8(std::filesystem::path& gamePath) {
    Chip8 chip8{};
    initFont(chip8);
    loadGameFile(gamePath, chip8);

    // Game instructions starts at 512th byte
    chip8.pc = 0x200;

    // seeding the prng
    chip8.prngSeed(static_cast<uint16_t>(std::time(nullptr)));

    return chip8;
}


/* takes in Chip8 struct
 * and initialize the std fonts from 0-F available by default on chip-8
 */
void initFont(Chip8& cpu) {
    std::array<std::uint8_t, 80> fonts {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
    std::memcpy(&cpu.memory[0], &fonts[0], 80);

}


/* takes in game-file path and chip8 struct
 * and loads that into interpreters memory
 * safely closes the game file
 */
void loadGameFile(fs::path& gamePath, Chip8& chip8) {
    std::ifstream file(gamePath, std::ios::binary);

    // Directly streams bytes into the array memory
    std::uint8_t* romDestination {chip8.memory.data() + 0x200};
    file.read(reinterpret_cast<char*>(romDestination), MAX_GAMEFILE_SIZE);

    //closing the file
    file.close();
}
