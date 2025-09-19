#pragma once
#include <vector>

// Define Chip-8 components
typedef unsigned char byte;
typedef unsigned short word;

class System
{
public:
    System();
    ~System();

    void LoadGame();
    word FetchObcode();
    void DecodeExecute(word instruction);

    bool ShouldDraw() const { return drawFlag; }
    const byte *GetDisplay() const { return display; }
    void ResetDrawFlag() { drawFlag = false; }

private:
    byte memory[4096]; // 4K memory
    byte V[16];        // 16 registers (V0 to VF)
    word I;            // Index register
    word pc;           // Program counter
    bool drawFlag;     // Indicates if the screen needs to be redrawn

    byte display[64 * 32]; // Display (64x32 pixels) (8 bytes x 4 bytes)
    std::vector<word> stack;
    int sp; // stack pointer

    byte delayTimer;
    byte soundTimer;
};
