//
// Created by amrit on 12/07/26.
//
#include "chip8.h"
#include <chrono>


int execute(Chip8& chip8) {
    using clock = std::chrono::steady_clock ;


    auto lastTimerTick{clock::now()};
    auto lastInstructionTimePoint{clock::now()};


    const auto timerInterval{std::chrono::duration<double>(1.0/60)};            // will run at 60Hz
    const auto instructionInterval{std::chrono::duration<double>(1.0/700)};     // cpu will run at 700Hz

    while (true) {
        // [implement event polling]


        auto now{clock::now()};


        while (now - lastInstructionTimePoint >= instructionInterval) {
            // [implement step function]
            lastInstructionTimePoint += std::chrono::duration_cast<clock::duration>( instructionInterval);
        }

        while (now - lastTimerTick>= timerInterval) {
            if (chip8.delayTimer != 0) chip8.delayTimer--;
            if (chip8.delayTimer != 0) chip8.delayTimer--;
            lastInstructionTimePoint += std::chrono::duration_cast<clock::duration>( instructionInterval);
        }

        

    }



}
