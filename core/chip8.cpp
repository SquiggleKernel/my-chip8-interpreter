//
// Created by amrit on 12/07/26.
//
#include "chip8.h"
#include <chrono>


void execute(Chip8& chip8,SdlObjects sdlObjects, Quirks& quirks, uint displayScaling) {
    using clock = std::chrono::steady_clock ;


    auto lastTimerTick{clock::now()};
    auto lastInstructionTimePoint{clock::now()};


    const auto timerInterval{std::chrono::nanoseconds(1'000'000'000/60)};            // timers will run at 60Hz
    const auto instructionInterval{std::chrono::nanoseconds(1'000'000'000/chip8.cpuSpeedHz)};     // cpu will run at 700Hz

    while (true) {
       switch (pollInput(sdlObjects, chip8)) {
           case 1:
                std::cerr << "Error occured while polling input\n";
                return;
           case 2:
               std::cout << "Quitting\n";
               return;
           default:
               break;
        }

        auto now{clock::now()};


        while (now - lastInstructionTimePoint >= instructionInterval) {
            step(chip8, quirks);
            lastInstructionTimePoint += std::chrono::duration_cast<clock::duration>( instructionInterval);
        }


        while (now - lastTimerTick>= timerInterval) {
            if (chip8.delayTimer != 0) chip8.delayTimer--;
            if (chip8.soundTimer != 0) chip8.soundTimer--;
            chip8.waitingForVblank = false;
            lastTimerTick += std::chrono::duration_cast<clock::duration>( timerInterval);
        }

        if (chip8.dirtyDisplay) {
            sdlRender(sdlObjects, chip8, displayScaling);
            chip8.dirtyDisplay = false;
        }
    }
}


void step(Chip8& cpu, Quirks quirks) {

    // getting the instruction to execute and increasing the program counter by 2
    Instructions instr{};
    instr.opcode = cpu.getOpcode();

    // // for debugging
    // std::cout<<"This instruction: " << std::hex << (int)instr.opcode << " is running\n registers ";
    // for (auto i: cpu.v_) {
    //     std::cout << (int)i << "  ";
    // }
    // std::cout << "I "<< cpu.indexRegister <<"pc:sp"<<cpu.pc << ":"<< (int)cpu.sp << '\n';
    // std::cout << "delayTime: " << (int)cpu.delayTimer << " : " << (int)cpu.soundTimer << '\n';


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
                cpu.dirtyDisplay = true;
            }
            else if (instr.opcode == 0x00EE) {
                cpu.sp--;
                cpu.pc = cpu.stack[cpu.sp];
                cpu.stack[cpu.sp] = 0;          // not necessary only for debugging
            }
            else {
                // nop for 0nnn instr
            }
            break;

        case 0x1:
            cpu.pc = instr.nnn();
            break;

        case 0x2:
            cpu.stack[cpu.sp] = cpu.pc;
            cpu.sp++;
            cpu.pc = instr.nnn();
            break;

        case 0x3:
            if (instr.kk() == vx) cpu.pc+=2;
            break;

        case 0x4:
            if (instr.kk() != vx) cpu.pc+=2;
            break;

        case 0x5:
            if (vy == vx) cpu.pc+=2;
            break;

        case 0x6:
            vx = instr.kk();
            break;

        case 0x7:
            temp = vx + instr.kk();
            vx = static_cast<uint8_t>(temp);
            break;

        // logical operations
        case 0x8:
            switch (instr.opcode & 0x000F) {
            case 0x0:
                    vx = vy;
                    break;
            case 0x1:
                    vx |= vy;
                    if (quirks.flagLogicalReset) cpu.v_[0xF] = 0;
                    break;
            case 0x2:
                    vx &= vy;
                    if (quirks.flagLogicalReset) cpu.v_[0xF] = 0;
                    break;
            case 0x3:
                    vx ^= vy;
                    if (quirks.flagLogicalReset) cpu.v_[0xF] = 0;
                    break;
            case 0x4:
                    temp = vx + vy;
                    vx = static_cast<uint8_t>(temp);
                    cpu.v_[0xF]= (temp >> 8) & 0b1;
                    break;
            case 0x5:
                    temp = vx;
                    vx -= vy;
                    cpu.v_[0xF]= (temp >= vy) ? 1 : 0 ;
                    break;
            case 0x6:
                    if (!quirks.shiftInPlace) {
                        vx = vy;
                    }
                    temp = vx;
                    vx >>= 1;
                    cpu.v_[0xF] = (temp & 0b1);
                    break;
            case 0x7:
                    temp = vx;
                    vx = vy-vx;
                    cpu.v_[0xF]= (vy >= temp) ? 1 : 0 ;
                    break;
            case 0xE:
                    if (!quirks.shiftInPlace) {
                        vx = vy;
                    }
                    temp = vx;
                    vx <<=1;
                    cpu.v_[0xF] = ((temp>>7) & 0b1);
                    break;
            default:
                    std::cerr << "This instruction \"" << std:: hex <<instr.opcode << "\" is not valid for 0x8 grp\n" << std::hex;
            }
            break;

        case 0x9:
            if (vy != vx) cpu.pc+=2;
            break;

        case 0xA:
            cpu.indexRegister = instr.nnn();
            break;

        case 0xB:
            cpu.pc = instr.nnn();
            if (quirks.jumpOffsetWithVx) {
                cpu.pc += vx;
            } else {
                cpu.pc += cpu.v_[0];
            }
            break;

        case 0xC:
            vx = instr.kk() & cpu.getRandomByte();
            break;

        case 0xD: {
            if (quirks.displayWait && cpu.waitingForVblank) {
                cpu.pc-=2;
                break;
            }
            cpu.v_[0xF] = 0;
            auto xStart{vx%64};
            auto yStart{vy%32};
            for (int j=0; j<instr.n() ; j++) {
                for (int i=0 ; i<8 ; i++) {

                    if (!quirks.clipping || ((j+yStart<32) && (i+xStart<64))) {

                        bool spritePixel {static_cast<bool>(((cpu.memory[static_cast<uint>(cpu.indexRegister+j)])>>(7-i))&1)};
                        bool& displayPixel {cpu.display[(uint)(yStart+j)%32][(uint)(xStart+i)%64]};

                        if (displayPixel & spritePixel) {
                            cpu.v_[0xF] = 1;
                        }

                        displayPixel ^= spritePixel;

                    } else {
                        // do nothing
                    }
                }
            }
            cpu.dirtyDisplay = true;
            if (quirks.displayWait) {
                cpu.waitingForVblank = true;
            }
            break;
        }
        case 0xE:
            if ((instr.opcode & 0xFF) == 0x9E) {
                if (cpu.keyboard[vx])   cpu.pc+=2;
            }
            else if ((instr.opcode & 0xFF) == 0xA1) {
                if (!cpu.keyboard[vx])   cpu.pc+=2;
            }
            break;

        case 0xF:
            switch (instr.opcode & 0x00FF) {
            case 0x07:
                    vx = cpu.delayTimer;
                    break;
            case 0x0A:
                    if (cpu.isKeyPressed) {
                        vx = cpu.lastPressedKey;
                        cpu.gotKey = false;
                    }
                    else {
                        if (!cpu.gotKey) {
                            cpu.gotKey = true;
                            break;
                        }
                    }
                    cpu.pc-=2;
                    break;
            case 0x15:
                    cpu.delayTimer = vx;
                    break;
            case 0x18:
                    cpu.soundTimer = vx;
                    break;
            case 0x1E:
                    cpu.indexRegister += vx;
                    break;
            case 0x29:
                    cpu.indexRegister = vx*5;
                    break;
            case 0x33:
                    temp = vx;
                    for (uint i{}; i<3 ; i++) {
                        cpu.memory[cpu.indexRegister+2-i] = static_cast<uint8_t>(temp%10);
                        temp/=10;
                    }

                    break;
            case 0x55:
                    temp = instr.x();
                    for (uint i=0; i<= temp; i++ ) {
                        cpu.memory[cpu.indexRegister+i] = cpu.v_[i];
                    }
                    if (quirks.registerIndexIncrement) {
                        cpu.indexRegister += temp;
                        cpu.indexRegister ++;
                    }
                    break;

            case 0x65:
                    temp = instr.x();
                    for (uint i=0; i<= temp; i++ ) {
                        cpu.v_[i] = cpu.memory[cpu.indexRegister+i];
                    }
                    if (quirks.registerIndexIncrement) {
                        cpu.indexRegister += temp;
                        cpu.indexRegister ++;
                    }
                    break;


            default:
                    std::cerr << "This instruction \"" << std:: hex <<instr.opcode << "\" is not valid for 0xF grp\n" << std::hex;
            }
            break;

        default:
            std::cerr << "This instruction: " << std::hex << instr.type() << instr.nnn() << "is not part of chip-8 instruction set.\n";


    }
}
