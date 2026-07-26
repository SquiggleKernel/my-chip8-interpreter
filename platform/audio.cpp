//
// Created by amrit on 13/07/26.
//
#include "window.h"
#include <cmath>
void sdlPlayBeep(SdlObjects& sdlObjects, bool isBeeping) {
    if (!sdlObjects.stream) return;

    if (isBeeping) {
        // Keep about 2048 bytes (~11ms) buffered.

        // (For 44.1 kHz, this equals 3,528 bytes; for 48 kHz, this equals 3,840 bytes)
        const int targetBufferBytes = static_cast<int>((float)sdlObjects.spec.freq * sizeof(float) * 0.020f);

        float buffer[256];
        const float frequency = 1000.0f;
        const float volume = 0.35f;      // 35% volume
        const float phaseInc = (2.0f * static_cast<float>(M_PI) * frequency) / static_cast<float>(sdlObjects.spec.freq);


        // If enough data is already queued, this loop exits instantly without blocking.
        while (SDL_GetAudioStreamQueued(sdlObjects.stream) < targetBufferBytes) {


            for (int i = 0; i < 256; ++i) {
                // SQUARE WAVE: If sine value is positive, play positive volume; if negative, play negative volume.
                float sample = (sdlObjects.audioPhase < static_cast<float>(M_PI)) ? volume : -volume;
                buffer[i] = sample;

                sdlObjects.audioPhase += phaseInc;
                if (sdlObjects.audioPhase >= 2.0f * static_cast<float>(M_PI)) {
                    sdlObjects.audioPhase -= 2.0f * static_cast<float>(M_PI);
                }
            }

            SDL_PutAudioStreamData(sdlObjects.stream, buffer, sizeof(buffer));
        }
    } else {
        // Stop audio instantly when soundTimer reaches 0
        if (SDL_GetAudioStreamQueued(sdlObjects.stream) > 0) {
            SDL_ClearAudioStream(sdlObjects.stream);
        }
    }
}