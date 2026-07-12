#include "core/initializationHelper.h"
#include "platform/window.h"

int main(int argc, char* argv[]) {

    if (argc!=2) {
        std::cerr<< "Usage: "<< argv[0]<< " <Game File path>\n";
        return 1;
    }

    // checking path validity
    fs::path romPath{argv[1]};
    if (!isPathValid(romPath)) {
        return 1;
    }
    Quirks quirks{};
    Chip8 chip8{initiateChip8(romPath)};

    uint displayScaling{15};
    SdlObjects sdlObjects{};

    if (sdlInit(sdlObjects, displayScaling) != 0) {
        std::cerr << "Error in while initializing SDL components\n";
        sdlCleanup(sdlObjects);
        return -1;
    }

    execute(chip8, sdlObjects, quirks, displayScaling);

    sdlCleanup(sdlObjects);
    return 0;
}
