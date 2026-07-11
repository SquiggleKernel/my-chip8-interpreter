//
// Created by amrit on 11/07/26.
//

#include "chip8.h"

//  check if path points to regular file and it is less than 3.5 KiB
bool isPathValid(const fs::path& romPath) {
    if (!fs::exists(romPath)) {
        std::cerr<< "file dont not exist\n";
        return false;
    }
    else if (!fs::is_regular_file(romPath)) {
        std::cerr << "not a file\n";
        return false;
    }
    if (fs::file_size(romPath)> MAX_GAMEFILE_SIZE) {
        std::cerr << "invalid rom [size too big]\n";
        return false;
    }
    return true;
}
