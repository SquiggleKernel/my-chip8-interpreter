//
// Created by amrit on 12/07/26.
//

#ifndef MY_CHIP8_INTERPRETER_WINDOW_H
#define MY_CHIP8_INTERPRETER_WINDOW_H

#include <SDL3/SDL.h>
#include <iostream>
struct Chip8;

struct SdlObjects {
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Event event{};
    SDL_AudioSpec spec{};
    SDL_AudioStream* stream{};

    float audioPhase{0.0f};
};


int sdlInit(SdlObjects& sdlObjects, uint displayScaling);
void sdlCleanup(SdlObjects& sdlObjects);
void sdlRender(SdlObjects& sdlObjects, Chip8& cpu ,uint displayScaling);

int pollInput(SdlObjects& sdlObjects, Chip8& chip8);

void sdlPlayBeep(SdlObjects& sdlObjects, bool isBeeping);

#endif //MY_CHIP8_INTERPRETER_WINDOW_H