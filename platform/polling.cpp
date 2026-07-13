//
// Created by amrit on 12/07/26.
//
#include "window.h"
#include "../core/chip8.h"

int pollInput(SdlObjects& sdlObjects,Chip8& chip8) {
    while (SDL_PollEvent(&sdlObjects.event)) {
        if (sdlObjects.event.type == SDL_EVENT_QUIT) {
            return 2;
        }

        // Check for key presses or releases
        if (sdlObjects.event.type == SDL_EVENT_KEY_DOWN || sdlObjects.event.type == SDL_EVENT_KEY_UP) {
            chip8.isKeyPressed = (sdlObjects.event.type == SDL_EVENT_KEY_DOWN);

            // Map SDL Scancodes to CHIP-8 Hex Keys
            switch (sdlObjects.event.key.scancode) {
                case SDL_SCANCODE_1: chip8.keyboard[0x1] = chip8.isKeyPressed; chip8.lastPressedKey= 0x1;  break;
                case SDL_SCANCODE_2: chip8.keyboard[0x2] = chip8.isKeyPressed; chip8.lastPressedKey= 0x2;  break;
                case SDL_SCANCODE_3: chip8.keyboard[0x3] = chip8.isKeyPressed; chip8.lastPressedKey= 0x3;  break;
                case SDL_SCANCODE_4: chip8.keyboard[0xC] = chip8.isKeyPressed; chip8.lastPressedKey= 0xC;  break;
                case SDL_SCANCODE_Q: chip8.keyboard[0x4] = chip8.isKeyPressed; chip8.lastPressedKey= 0x4;  break;
                case SDL_SCANCODE_W: chip8.keyboard[0x5] = chip8.isKeyPressed; chip8.lastPressedKey= 0x5;  break;
                case SDL_SCANCODE_E: chip8.keyboard[0x6] = chip8.isKeyPressed; chip8.lastPressedKey= 0x6;  break;
                case SDL_SCANCODE_R: chip8.keyboard[0xD] = chip8.isKeyPressed; chip8.lastPressedKey= 0xD;  break;
                case SDL_SCANCODE_A: chip8.keyboard[0x7] = chip8.isKeyPressed; chip8.lastPressedKey= 0x7;  break;
                case SDL_SCANCODE_S: chip8.keyboard[0x8] = chip8.isKeyPressed; chip8.lastPressedKey= 0x8;  break;
                case SDL_SCANCODE_D: chip8.keyboard[0x9] = chip8.isKeyPressed; chip8.lastPressedKey= 0x9;  break;
                case SDL_SCANCODE_F: chip8.keyboard[0xE] = chip8.isKeyPressed; chip8.lastPressedKey= 0xE;  break;
                case SDL_SCANCODE_Z: chip8.keyboard[0xA] = chip8.isKeyPressed; chip8.lastPressedKey= 0xA;  break;
                case SDL_SCANCODE_X: chip8.keyboard[0x0] = chip8.isKeyPressed; chip8.lastPressedKey= 0x0;  break;
                case SDL_SCANCODE_C: chip8.keyboard[0xB] = chip8.isKeyPressed; chip8.lastPressedKey= 0xB;  break;
                case SDL_SCANCODE_V: chip8.keyboard[0xF] = chip8.isKeyPressed; chip8.lastPressedKey= 0xF;  break;

                case SDL_SCANCODE_ESCAPE: return 1;
                default: break;
            }
        }
    }
    return 0;
}