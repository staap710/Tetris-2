#ifndef TETRIS_H
#define TETRIS_H

#include "raylib.h"
#include <vector>
#include <string>

class Tetris {
public:
    Tetris(int screenWidth, int screenHeight);
    void Update();
    void Draw();
    bool GameOver();

private:
    static const int BOARD_WIDTH = 10;
    static const int BOARD_HEIGHT = 20;
    static const int BLOCK_SIZE = 30;
    const float LOCK_DELAY_TIME = 0.5f;

    struct Piece {
        std::vector<std::vector<int>> shape;
        Color color;
        int x, y;
    };

    int board[BOARD_WIDTH][BOARD_HEIGHT];
    Piece currentPiece;
    Piece nextPiece;

    int score = 0;
    float lockDelayTimer = 0.0f;
    bool isPieceAboutToLock = false;

    void InitBoard();
    void SpawnPiece();
    void DrawNextPiece();
    bool IsValidMove(int offsetX, int offsetY);
    void RotatePiece();
    void MovePiece(int offsetX, int offsetY);
    void LockPiece();
    void ClearLines();
    Piece GetRandomPiece();
};

#endif // TETRIS_H