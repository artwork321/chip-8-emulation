#include <vector>
#include <fstream>
#include <iostream>
#include "system.h"

// Load game into memory starting at 0x200 (512/4095)
void System::LoadGame()
{
    // Read file
    std::ifstream rom{"2-ibm-logo.ch8", std::ios::binary | std::ios::ate};

    if (!rom)
    {
        std::cerr << "Failed to open ROM file." << std::endl;
        return;
    }

    // Load all data into memory starting at 0x200
    std::streamsize size = rom.tellg();
    rom.seekg(0, std::ios::beg);
    rom.read(reinterpret_cast<char *>(&memory[0x200]), size);

    std::cout << "Loaded " << size << " bytes, example: " << static_cast<int>(memory[0x200]) << ".\n";

    // Set program counter to 0x200
    pc = 0x200;
}

// Read instruction at pc and increment pc
word System::FetchObcode()
{
    byte firstHalf = memory[pc];
    byte secondHalf = memory[pc + 1];
    pc += 2;
    word instruction{static_cast<word>(firstHalf << 8 | secondHalf)};
    return instruction;
}

void System::DecodeExecute(word instruction)
{

    // Get components
    word firstNibble = instruction & 0xF000;
    word secondNibble = instruction & 0x0F00;
    word thirdNibble = instruction & 0x00F0;
    word lastNibble = instruction & 0x000F;

    // 00E0: clear screen
    switch (firstNibble)
    {
    case 0x0000:
        if (instruction == 0x00E0)
        {
            // Clear display
            for (int i = 0; i < 64 * 32; ++i)
            {
                display[i] = 0;
            }
        }
        break;
    // 1NNN: jump to address NNN
    case 0x1000:
    {
        word address = instruction & 0x0FFF;
        pc = address;
        break;
    }

    // 6XNN (set register VX)
    case 0x6000:
    {
        V[secondNibble >> 8] = instruction & 0x00FF;
        break;
    }

    // 7XNN (add value to register VX)
    case 0x7000:
    {
        V[secondNibble >> 8] += instruction & 0x00FF;
        break;
    }

    // ANNN (set index register I)
    case 0xA000:
    {
        I = instruction & 0x0FFF;
        break;
    }

    // DXYN (display/draw)
    case 0xD000:
    {
        // Extract coordinates
        byte x = V[secondNibble >> 8];
        byte y = V[thirdNibble >> 4];
        byte height = lastNibble;

        // Draw sprite at (x, y) with height N
        V[0xF] = 0; // Reset collision flag

        for (int j = 0; j < height; ++j)
        {
            // XOR each sprite pixel with the screen; only when a sprite pixel of 1 turns a screen pixel from 1 → 0 is VF set.
            // Otherwise, pixels are either turned on or unchanged.
            byte sprite = memory[I + j];

            for (int i = 0; i < 8; ++i)
            {
                byte pixel = (sprite >> (7 - i)) & 1;

                if (pixel == 1 && display[x + y * 64 + i] == 1)
                {
                    V[0xF] = 1; // Set collision flag
                }

                display[x + y * 64 + i] ^= pixel;
            }
        }
        drawFlag = true; // Indicate that the screen needs to be redrawn
        break;
    }
    }
}

System::System()
    : I{0}, pc{0x200}, drawFlag{false}, sp{0}, delayTimer{0}, soundTimer{0}
{
    // Initialize memory and registers to 0
    for (int i = 0; i < 4096; ++i)
    {
        memory[i] = 0;
    }
    for (int i = 0; i < 16; ++i)
    {
        V[i] = 0;
    }
    for (int i = 0; i < 64 * 32; ++i)
    {
        display[i] = 0;
    }
}
