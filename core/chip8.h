//
// Created by amrit on 11/07/26.
//

#ifndef MY_CHIP8_INTERPRETER_CHIP8_H
#define MY_CHIP8_INTERPRETER_CHIP8_H

#include <array>
#include <iostream>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <cstring>

struct Chip8 {
    // 4Kib of RAM, 16 8 bit registers and 16 bit Index register
    std::array<uint8_t, 4096> memory{};
    std::array<uint8_t, 16> v_{};
    uint16_t indexRegister{};

    //program counter and stack pointer
    uint16_t pc{0x200};
    uint8_t sp{};

    // 16 level stack
    std::array<uint16_t, 16 > stack{};

    // timers
    uint8_t delayTimer{0};
    uint8_t soundTimer{0};

    // display
    std::array<std::array<bool, 64>, 32> display{};
    bool dirtyDisplay{true};

    // keyboard
    std::array<bool,16> keyboard{};

    // PRNG
    uint16_t prngState{};

    // Call this once when the emulator starts
    void prngSeed(uint16_t val) {
        // State cannot be 0, or the math will always return 0
        prngState = (val == 0) ? 0xACE1 : val;
    }

    // Generates the next number
    uint8_t get_byte() {
        prngState ^= prngState << 7;
        prngState ^= prngState >> 9;
        prngState ^= prngState << 8;
        return static_cast<std::uint8_t>(prngState & 0xFF);
    }

    uint16_t getOpcode() {
        return static_cast<uint16_t>( memory[pc]<<8 | memory [pc+1]);
    }



};

struct Instructions {
    std::uint16_t opcode{};
    // Common Chip-8 components decoded from the opcode
    uint8_t type() const { return static_cast<uint8_t>((opcode & 0xF000) >> 12);    } // First nibble
    uint8_t x()    const { return static_cast<uint8_t>((opcode & 0x0F00) >> 8);     } // Second nibble or lower 4 bit of 1st byte
    uint8_t y()    const { return static_cast<uint8_t>((opcode & 0x00F0) >> 4);     } // Third nibble or upper 4 bit of 2nd byte
    uint8_t n()    const { return static_cast<uint8_t>((opcode & 0x000F));          } // Fourth nibble
    uint8_t kk()   const { return static_cast<uint8_t>((opcode & 0x00FF));          } // last byte of the instruction
    uint16_t nnn() const { return static_cast<uint16_t>((opcode & 0x0FFF));         } // Last 12 bits
};



struct Quirks {
    bool shiftInstruction {true};       // true on original chip-8
    bool registerIndexIncrement{true};  // true on original chip-8
    bool jumpWithOffset{true};          // tells what offset to use during jump instruction, V0 on original, Vx on new
    bool flagInstructionReset{true};    // true on original chip-8
    bool clipping{true};                // true on original chip-8
};

void step(Chip8& cpu, Quirks quirks);
int execute(Chip8& chip8, Quirks& quirks);



#endif //MY_CHIP8_INTERPRETER_CHIP8_H
