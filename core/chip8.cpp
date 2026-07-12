//
// Created by amrit on 12/07/26.
//
#include "chip8.h"
#include <chrono>

void notImplementedYet(Instructions& instr) {
    std::cerr << "This instruction: " << std::hex << instr.type() << instr.nnn() << "has not been implemented yet.\n";
}


int execute(Chip8& chip8, Quirks& quirks) {
    using clock = std::chrono::steady_clock ;


    auto lastTimerTick{clock::now()};
    auto lastInstructionTimePoint{clock::now()};


    const auto timerInterval{std::chrono::duration<double>(1.0/60)};            // will run at 60Hz
    const auto instructionInterval{std::chrono::duration<double>(1.0/700)};     // cpu will run at 700Hz

    while (true) {
        // [implement event polling]


        auto now{clock::now()};


        while (now - lastInstructionTimePoint >= instructionInterval) {
            step(chip8, quirks);
            lastInstructionTimePoint += std::chrono::duration_cast<clock::duration>( instructionInterval);
        }

        while (now - lastTimerTick>= timerInterval) {
            if (chip8.delayTimer != 0) chip8.delayTimer--;
            if (chip8.delayTimer != 0) chip8.delayTimer--;
            lastInstructionTimePoint += std::chrono::duration_cast<clock::duration>( instructionInterval);
        }

        if (chip8.dirtyDisplay) {
            // [render new frame]
            chip8.dirtyDisplay = false;
        }
    }
    return 0;
}


void step(Chip8& cpu, Quirks quirks) {

    // getting the instruction to execute and increasing the program counter by 2
    Instructions instr{};
    instr.opcode = cpu.getOpcode();
    cpu.pc+=2;

    uint16_t temp{};
    uint8_t& vx = cpu.v_[instr.x()];
    uint8_t& vy = cpu.v_[instr.y()];

    // giant opcode switch statement
    switch ((instr.opcode & 0xF000) >> 12) {
        case 0x0:
            if (instr.opcode == 0x00E0) {
                for (uint i{0} ; i<32; i++) {
                    for (uint j{0}; j< 64 ; j++) {
                        cpu.display[i][j] = false;
                    }
                }
            }
            else if (instr.opcode == 0x00EE) {
                // implement it
                notImplementedYet(instr);
            }
            else {
                // nop for 0nnn instr
            }
            break;

        case 0x1:
            cpu.pc = instr.nnn();
            break;

        case 0x2:
            // implement it
            notImplementedYet(instr);
            break;

        case 0x3:
            // implement it
            notImplementedYet(instr);
            break;

        case 0x4:
            // implement it
            notImplementedYet(instr);
            break;

        case 0x5:
            // implement it
            notImplementedYet(instr);
            break;

        case 0x6:
            vx = instr.kk();
            break;

        case 0x7:
            temp = vx + instr.kk();
            if (temp > 255) cpu.v_[0xF] = 1;
            vx = static_cast<uint8_t>(temp);
            break;

        case 0x8:
            // implement it
            notImplementedYet(instr);
            break;

        case 0x9:
            // implement it
            notImplementedYet(instr);
            break;

        case 0xA:
            cpu.indexRegister = instr.nnn();
            break;

        case 0xB:
            // implement it
            notImplementedYet(instr);
            break;

        case 0xC:
            // implement it
            notImplementedYet(instr);
            break;

        case 0xD:
            cpu.v_[0xF] = 0;
            for (int j=0 ; j<instr.n() ; j++) {
                for (int i=0 ; i<8 ; i++) {
                    if (!quirks.clipping  || ((j+vy)< 64) & ((i+vx)<32)) {

                        bool& pixel {cpu.display[(j+vy)&63][(i+vx)&31]};
                        bool switchTo{static_cast<bool>(cpu.indexRegister + i + j)};

                        pixel ^= switchTo;
                        cpu.v_[0xF] = (!pixel)&(switchTo);

                    } else {
                        // do nothing in case if clipping is enabled and writting outside screen
                    }
                }
            }
            cpu. dirtyDisplay = true;
            break;

        case 0xE:
            // implement it
            notImplementedYet(instr);
            break;

        case 0xF:
            // implement it
            notImplementedYet(instr);
            break;

        default:
            std::cerr << "This instruction: " << std::hex << instr.type() << instr.nnn() << "is not part of chip-8 instruction set.\n";


    }
}
