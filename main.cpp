#include "core/initializationHelper.h"

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

    Chip8 chip8{initiateChip8(romPath)};
    

    return 0;
}