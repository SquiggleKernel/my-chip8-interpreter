//
// Created by amrit on 12/07/26.
//
#include "window.h"
#include "../core/chip8.h"

int pollInput(SdlObjects& sdlObjects,Chip8& chip8) {
    while (SDL_PollEvent(&sdlObjects.event)) {
        if (sdlObjects.event.type == SDL_EVENT_QUIT) {
            return -1;
        }

        // Check for key presses or releases
        if (sdlObjects.event.type == SDL_EVENT_KEY_DOWN || sdlObjects.event.type == SDL_EVENT_KEY_UP) {
            bool isPressed = (sdlObjects.event.type == SDL_EVENT_KEY_DOWN);

            // Map SDL Scancodes to CHIP-8 Hex Keys
            switch (sdlObjects.event.key.scancode) {
                case SDL_SCANCODE_1: chip8.keyboard[0x1] = isPressed; break;
                case SDL_SCANCODE_2: chip8.keyboard[0x2] = isPressed; break;
                case SDL_SCANCODE_3: chip8.keyboard[0x3] = isPressed; break;
                case SDL_SCANCODE_4: chip8.keyboard[0xC] = isPressed; break;

                case SDL_SCANCODE_Q: chip8.keyboard[0x4] = isPressed; break;
                case SDL_SCANCODE_W: chip8.keyboard[0x5] = isPressed; break;
                case SDL_SCANCODE_E: chip8.keyboard[0x6] = isPressed; break;
                case SDL_SCANCODE_R: chip8.keyboard[0xD] = isPressed; break;

                case SDL_SCANCODE_A: chip8.keyboard[0x7] = isPressed; break;
                case SDL_SCANCODE_S: chip8.keyboard[0x8] = isPressed; break;
                case SDL_SCANCODE_D: chip8.keyboard[0x9] = isPressed; break;
                case SDL_SCANCODE_F: chip8.keyboard[0xE] = isPressed; break;

                case SDL_SCANCODE_Z: chip8.keyboard[0xA] = isPressed; break;
                case SDL_SCANCODE_X: chip8.keyboard[0x0] = isPressed; break;
                case SDL_SCANCODE_C: chip8.keyboard[0xB] = isPressed; break;
                case SDL_SCANCODE_V: chip8.keyboard[0xF] = isPressed; break;

                case SDL_SCANCODE_ESCAPE: return -1;
                default: break;
            }
        }
    }
    return 0;
}