#include <vector>
#include <fstream>
#include <iostream>

// Define Chip-8 components
typedef unsigned char byte;
typedef unsigned short word;

byte memory[4096]; // 4K memory
byte V[16];        // 16 registers (V0 to VF)
word I;            // Index register
word pc;           // Program counter

byte display[64 * 32]; // Display (64x32 pixels)
std::vector<word> stack;
int sp; // stack pointer

byte delayTimer;
byte soundTimer;

// Load game into memory starting at 0x200 (512/4095)
void LoadGame()
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
    rom.read(reinterpret_cast<char *>(&memory[512]), size);

    std::cout << "Loaded " << size << " bytes, example: " << static_cast<int>(memory[512]) << ".\n";

    // Set program counter to 0x200
    pc = 512;
}

// Read instruction at pc and increment pc
word FetchObcode()
{
    byte firstHalf = memory[pc];
    byte secondHalf = memory[pc + 1];
    pc += 2;
    word instruction{firstHalf << 8 | secondHalf};
    return instruction;
}

void Decode(word instruction)
{

    switch (instruction)
    {
    // 00E0 (clear screen)
    case 0x00E0:
        for (int i = 0; i < 64 * 32; ++i)
        {
            display[i] = 0;
        }
        break;
        // 1NNN (jump)
        // 6XNN (set register VX)
        // 7XNN (add value to register VX)
        // ANNN (set index register I)
        // DXYN (display/draw)
    }
}

int main()
{
    LoadGame();
    return 0;
}