#include "raylib.h"
#include "resource_dir.h"
#include "system.h"
#include <iostream>

constexpr int cellSize = 25;
constexpr int gridWidth = 64;
constexpr int gridHeight = 32;

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window
	InitWindow(gridWidth * cellSize, gridHeight * cellSize, "Chip 8 Emulator - Raylib");
	// Cap framerate to make behavior deterministic and reduce CPU spin
	SetTargetFPS(60);

	// Load chip8 system
	System chip8{System()};
	chip8.LoadGame();

	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		chip8.OneCycle();

		if (chip8.ShouldDraw())
		{
			for (int i = 0; i < 64; ++i)
			{
				for (int j = 0; j < 32; ++j)
				{
					if (chip8.GetDisplay()[j * 64 + i] == 1)
					{
						DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, WHITE);
					}
				}
			}
			// chip8.ResetDrawFlag();
		}

		// Debug: draw a small test rectangle and drawFlag text so we can verify drawing works
		DrawRectangle(0, 0, cellSize, cellSize, RED);
		DrawText(TextFormat("drawFlag: %d", chip8.ShouldDraw()), cellSize + 4, 4, 20, GREEN);

		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
