#include "raylib.h"
#include "Tetris.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 600

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");
    SetTargetFPS(60);

    Tetris tetris(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!WindowShouldClose()) {

        tetris.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        tetris.Draw();
        EndDrawing();

        if (tetris.GameOver()) {
            break;
        }
    }

    CloseWindow();
    return 0;
}