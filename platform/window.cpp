//
// Created by amrit on 12/07/26.
//
#include "window.h"
#include "../core/chip8.h"

int sdlInit(SdlObjects& sdlObjects, uint displayScaling) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("[ERROR]: SDL Init Failed!");
        return 2;
    }


    sdlObjects.window = SDL_CreateWindow("Chip8 Interpreter", (int)(64*displayScaling), (int)(32*displayScaling), SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE);
    sdlObjects.renderer = SDL_CreateRenderer(sdlObjects.window,NULL);
    if (!sdlObjects.renderer) {
        std::cerr << "[ERROR]: " << SDL_GetError() << '\n';
        return 2;
    }
    return 0;
}

void sdlRender(SdlObjects& sdlObjects, Chip8& cpu ,uint displayScaling) {
    SDL_SetRenderDrawColor(sdlObjects.renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlObjects.renderer); // Wipe the back buffer


    //for every pixel
    for (uint i{0}; i<64 ; i++) {
        for (uint j{0}; j< 32; j++) {
            float x{static_cast<float>(i*displayScaling)};
            float y{static_cast<float>(j*displayScaling)};
            float scaling{static_cast<float>(displayScaling)};
            SDL_FRect rect{x, y ,scaling, scaling };

            bool pixelColor{cpu.display[j][i]};
            if (pixelColor)
                SDL_SetRenderDrawColor(sdlObjects.renderer, 0x0F,0x38, 0x0F, 255 );
            else
                SDL_SetRenderDrawColor(sdlObjects.renderer, 0x8B,0xAC, 0x0F, 255 );
            SDL_RenderFillRect(sdlObjects.renderer, &rect);
        }
    }
    SDL_RenderPresent(sdlObjects.renderer);
}

void sdlCleanup(SdlObjects& sdlObjects) {
    if (sdlObjects.renderer) {
        SDL_DestroyRenderer(sdlObjects.renderer);
        sdlObjects.renderer = NULL;
    }
    if (sdlObjects.window) {
        SDL_DestroyWindow(sdlObjects.window);
        sdlObjects.window = NULL;
    }
    SDL_Quit();
}
