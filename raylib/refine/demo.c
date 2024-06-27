#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BLOCK_SIZE 30
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define MAX_BLOCKS 4

typedef struct {
    Vector2 blocks[MAX_BLOCKS];
    Color color;
} Tetromino;

const Tetromino TETROMINOES[] = {
    {{{0,0}, {1,0}, {2,0}, {3,0}}, SKYBLUE}, // I
    {{{0,0}, {1,0}, {0,1}, {1,1}}, YELLOW},  // O
    {{{1,0}, {0,1}, {1,1}, {2,1}}, PURPLE},  // T
    {{{1,0}, {2,0}, {0,1}, {1,1}}, GREEN},   // S
    {{{0,0}, {1,0}, {1,1}, {2,1}}, RED},     // Z
    {{{0,0}, {0,1}, {1,1}, {2,1}}, BLUE},    // J
    {{{2,0}, {0,1}, {1,1}, {2,1}}, ORANGE}   // L
};

Color board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
Tetromino currentPiece;
Vector2 currentPosition;
float fallTime = 0;
float fallSpeed = 1.0f;

void SpawnNewPiece(void) {
    currentPiece = TETROMINOES[GetRandomValue(0, 6)];
    currentPosition = (Vector2){BOARD_WIDTH / 2 - 1, 0};
}

bool CheckCollision(void) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        int x = currentPosition.x + currentPiece.blocks[i].x;
        int y = currentPosition.y + currentPiece.blocks[i].y;
        
        if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT || (y >= 0 && board[y][x].a != 0)) {
            return true;
        }
    }
    return false;
}

void LockPiece(void) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        int x = currentPosition.x + currentPiece.blocks[i].x;
        int y = currentPosition.y + currentPiece.blocks[i].y;
        if (y >= 0) {
            board[y][x] = currentPiece.color;
        }
    }
}

void ClearLines(void) {
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool lineFull = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x].a == 0) {
                lineFull = false;
                break;
            }
        }
        if (lineFull) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board[yy][x] = board[yy - 1][x];
                }
            }
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[0][x] = (Color){0};
            }
            y++;  // Recheck the same line
        }
    }
}

void RotatePiece(void) {
    Tetromino rotated = currentPiece;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        float x = rotated.blocks[i].x;
        rotated.blocks[i].x = -rotated.blocks[i].y;
        rotated.blocks[i].y = x;
    }
    
    Tetromino temp = currentPiece;
    currentPiece = rotated;
    if (CheckCollision()) {
        currentPiece = temp;  // Revert if invalid
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Tetris");
    SetTargetFPS(60);

    SpawnNewPiece();

    while (!WindowShouldClose()) {
        fallTime += GetFrameTime();
        if (fallTime >= fallSpeed) {
            fallTime = 0;
            currentPosition.y++;
            if (CheckCollision()) {
                currentPosition.y--;
                LockPiece();
                ClearLines();
                SpawnNewPiece();
            }
        }

        if (IsKeyPressed(KEY_LEFT)) currentPosition.x--;
        if (IsKeyPressed(KEY_RIGHT)) currentPosition.x++;
        if (IsKeyPressed(KEY_UP)) RotatePiece();
        if (IsKeyPressed(KEY_DOWN)) fallSpeed = 0.1f;

        if (CheckCollision()) {
            if (IsKeyPressed(KEY_LEFT)) currentPosition.x++;
            if (IsKeyPressed(KEY_RIGHT)) currentPosition.x--;
            fallSpeed = 1.0f;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw board
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                DrawRectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, board[y][x]);
            }
        }

        // Draw current piece
        for (int i = 0; i < MAX_BLOCKS; i++) {
            DrawRectangle((currentPosition.x + currentPiece.blocks[i].x) * BLOCK_SIZE, 
                          (currentPosition.y + currentPiece.blocks[i].y) * BLOCK_SIZE, 
                          BLOCK_SIZE - 1, BLOCK_SIZE - 1, currentPiece.color);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
