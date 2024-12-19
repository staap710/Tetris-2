#include "Tetris.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

Tetris::Tetris(int screenWidth, int screenHeight) {
    srand(time(nullptr));
    InitBoard();
    
    // Generate first two pieces
    currentPiece = GetRandomPiece();
    nextPiece = GetRandomPiece();
}

void Tetris::InitBoard() {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            board[x][y] = 0;
        }
    }
}

void Tetris::SpawnPiece() {
    // Use the pre-generated next piece as current piece
    currentPiece = nextPiece;
    
    // Generate a new next piece
    nextPiece = GetRandomPiece();
    
    currentPiece.x = BOARD_WIDTH / 2 - currentPiece.shape[0].size() / 2;
    currentPiece.y = 0;

    if (!IsValidMove(0, 0)) {
        // Game Over condition
        std::cout << "Game Over!" << std::endl;
    }
}

Tetris::Piece Tetris::GetRandomPiece() {
    Piece pieces[] = {                                        //https://www.reddit.com/r/Tetris/comments/6m85oq/what_are_the_colours_of_he_original_tetris_blocks/#:~:text=L%20piece%20is%20purple%2C%20I,games%20usually%20use%20rainbow%20colors).
        // I piece
        {{{{1,1,1,1}}}, BLUE, 0, 0},
        // O piece
        {{{{1,1},{1,1}}}, YELLOW, 0, 0},
        // T piece
        {{{{0,1,0},{1,1,1}}}, PURPLE, 0, 0},
        // L piece
        {{{{1,0,0},{1,1,1}}}, ORANGE, 0, 0},
        // J piece
        {{{{0,0,1},{1,1,1}}}, DARKBLUE, 0, 0},
        // S piece
        {{{{0,1,1},{1,1,0}}}, GREEN, 0, 0},
        // Z piece
        {{{{1,1,0},{0,1,1}}}, RED, 0, 0}
    };

    return pieces[rand() % 7];
}

bool Tetris::IsValidMove(int offsetX, int offsetY) {
    for (size_t y = 0; y < currentPiece.shape.size(); y++) {
        for (size_t x = 0; x < currentPiece.shape[y].size(); x++) {
            if (currentPiece.shape[y][x]) {
                int newX = currentPiece.x + x + offsetX;
                int newY = currentPiece.y + y + offsetY;

                if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT) {
                    return false;
                }

                if (newY >= 0 && board[newX][newY]) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Tetris::RotatePiece() {
    Piece rotatedPiece = currentPiece;
    std::vector<std::vector<int>> newShape(currentPiece.shape[0].size(),
        std::vector<int>(currentPiece.shape.size()));

    // Rotate the piece
    for (size_t y = 0; y < currentPiece.shape.size(); y++) {
        for (size_t x = 0; x < currentPiece.shape[y].size(); x++) {
            newShape[x][currentPiece.shape.size() - 1 - y] = currentPiece.shape[y][x];
        }
    }

    rotatedPiece.shape = newShape;

    // Wall kick attempts (try moving piece to make rotation valid)
    int kickOffsets[] = { 0, -1, 1, -2, 2 };
    bool rotationSuccessful = false;

    for (int offsetX : kickOffsets) {
        for (int offsetY : kickOffsets) {
            // Temporarily modify piece position                                                            //
            rotatedPiece.x = currentPiece.x + offsetX;
            rotatedPiece.y = currentPiece.y + offsetY;

            // Check if this rotated and shifted position is valid
            bool valid = true;
            for (size_t y = 0; y < rotatedPiece.shape.size(); y++) {
                for (size_t x = 0; x < rotatedPiece.shape[y].size(); x++) {
                    if (rotatedPiece.shape[y][x]) {
                        int newX = rotatedPiece.x + x;
                        int newY = rotatedPiece.y + y;

                        if (newX < 0 || newX >= BOARD_WIDTH ||
                            newY < 0 || newY >= BOARD_HEIGHT ||
                            (newY >= 0 && board[newX][newY])) {
                            valid = false;
                            break;
                        }
                    }
                }
                if (!valid) break;
            }

            // If valid position found, update piece
            if (valid) {
                currentPiece = rotatedPiece;
                rotationSuccessful = true;
                break;
            }
        }
        if (rotationSuccessful) break;
    }
}

void Tetris::MovePiece(int offsetX, int offsetY) {
    if (IsValidMove(offsetX, offsetY)) {

        currentPiece.x += offsetX;
        currentPiece.y += offsetY;

    }
}

void Tetris::LockPiece() {
    for (size_t y = 0; y < currentPiece.shape.size(); y++) {
        for (size_t x = 0; x < currentPiece.shape[y].size(); x++) {
            if (currentPiece.shape[y][x]) {
                //WaitTime(0.05); I can trry using this so it doesn't lock instantly
                board[currentPiece.x + x][currentPiece.y + y] = 1;
            }
        }
    }
    ClearLines();
    SpawnPiece();
}

void Tetris::ClearLines() {

    int linesCleared = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool lineFull = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!board[x][y]) {
                lineFull = false;
                break;
            }
        }

        if (lineFull) {
            for (int k = y; k > 0; k--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board[x][k] = board[x][k - 1];
                }
            }
            y++; // Check the same line again
            linesCleared++;
            if (linesCleared == 3) { lineFull = true; };
        }
    }

    // Add score for cleared lines
    if (linesCleared > 0) {
        score += 100 * linesCleared;
    }
    if (linesCleared == 3) {
        score += 200;
    }

}

void Tetris::Update() {


    if (IsKeyPressed(KEY_LEFT)) {
        MovePiece(-1, 0);
        lockDelayTimer = 0.0f;  //reset the lock delay timer after moving
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        MovePiece(1, 0);
        lockDelayTimer = 0.0f;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        MovePiece(0, 1);
        lockDelayTimer = 0.0f;
    }
    if (IsKeyPressed(KEY_UP)) {
        RotatePiece();
        lockDelayTimer = 0.0f;
    }

    // Automatic piece dropping
    static float dropTimer = 0.0f;
    dropTimer += GetFrameTime();

    if (dropTimer >= 0.5f) {  // Adjust speed here (0.5 seconds per drop)
        if (IsValidMove(0, 1)) {
            MovePiece(0, 1);
        }
        dropTimer = 0.0f;
    }

    // Lock delay mechanism
    if (!IsValidMove(0, 1)) {
        if (!isPieceAboutToLock) {
            isPieceAboutToLock = true;
            lockDelayTimer = 0.0f;
        }

        lockDelayTimer += GetFrameTime();  // Increment timer

        // If lock delay time exceeded, lock the piece
        if (lockDelayTimer >= LOCK_DELAY_TIME) {
            LockPiece();
            SpawnPiece();
            isPieceAboutToLock = false;
            lockDelayTimer = 0.0f;
        }
    }
    else {
        // If piece can move down, reset lock delay
        isPieceAboutToLock = false;
        lockDelayTimer = 0.0f;
    }
}

void Tetris::Draw() {
    // Draw board
    std::string scoreText = "Your score is: " + std::to_string(score);
    DrawText(scoreText.c_str(), 300, 10, 5, WHITE);
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (board[x][y]) {
                DrawRectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, GRAY);
            }
            DrawRectangleLines(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, DARKGRAY);
        }
    }

    // Draw current piece
    for (size_t y = 0; y < currentPiece.shape.size(); y++) {
        for (size_t x = 0; x < currentPiece.shape[y].size(); x++) {
            if (currentPiece.shape[y][x]) {
                DrawRectangle((currentPiece.x + x) * BLOCK_SIZE,
                    (currentPiece.y + y) * BLOCK_SIZE,
                    BLOCK_SIZE, BLOCK_SIZE, currentPiece.color);
            }
        }
    }
}

bool Tetris::GameOver() {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (board[x][0]) return true;
    }
    return false;
}