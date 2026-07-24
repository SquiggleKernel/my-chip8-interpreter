# my-chip8-interpreter

A CHIP-8 interpreter written in C++20, using SDL3 for the display, input, and audio.

I built this as a learning project to understand emulator architecture and low-level systems programming - decoding opcodes, managing memory/registers, and hooking up a real-time render/input loop.

## Features
- Core CHIP-8 instruction set implementation (`core/chip8.cpp`)
- SDL3-based window, rendering, input polling, and audio (`platform/`)
- Configurable display scaling
- Quirks handling for compatibility with different CHIP-8 ROM behaviors

## Requirements
- CMake 3.20+
- A C++20 compiler
- SDL3 

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

```bash
./my_chip8_interpreter <path-to-rom>
```

## Status

Still a work in progress — built mainly for learning purposes. Feedback and suggestions are welcome.
