#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_SIZE 20
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct {
    int x, y;
    int shape[4][4];
} Tetromino;

const int shapes[7][4][4] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
    {{1, 1}, {1, 1}}, // O
    {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}, // T
    {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}, // S
    {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}, // Z
    {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}, // J
    {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}  // L
};

int board[BOARD_WIDTH][BOARD_HEIGHT] = {0};

Tetromino currentTetromino;
int speed = 20;
int frameCount = 0;

void InitTetromino() {
    int shapeIndex = rand() % 7;
    currentTetromino.x = BOARD_WIDTH / 2 - 2;
    currentTetromino.y = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentTetromino.shape[i][j] = shapes[shapeIndex][i][j];
}

void RotateTetromino() {
    int temp[4][4] = {0};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[j][3 - i] = currentTetromino.shape[i][j];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentTetromino.shape[i][j] = temp[i][j];
}

bool CheckCollision(int newX, int newY) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentTetromino.shape[i][j])
                if (newX + i < 0 || newX + i >= BOARD_WIDTH || newY + j >= BOARD_HEIGHT || board[newX + i][newY + j])
                    return true;
    return false;
}

void PlaceTetromino() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentTetromino.shape[i][j])
                board[currentTetromino.x + i][currentTetromino.y + j] = 1;
    InitTetromino();
}

void UpdateBoard() {
    if (++frameCount >= speed) {
        frameCount = 0;
        if (!CheckCollision(currentTetromino.x, currentTetromino.y + 1))
            currentTetromino.y++;
        else
            PlaceTetromino();
    }
}

void DrawBoard() {
    ClearBackground(RAYWHITE);
    for (int x = 0; x < BOARD_WIDTH; x++)
        for (int y = 0; y < BOARD_HEIGHT; y++)
            if (board[x][y])
                DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, GRAY);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentTetromino.shape[i][j])
                DrawRectangle((currentTetromino.x + i) * GRID_SIZE, (currentTetromino.y + j) * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");
    SetTargetFPS(60);
    srand(time(NULL));

    InitTetromino();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_LEFT) && !CheckCollision(currentTetromino.x - 1, currentTetromino.y))
            currentTetromino.x--;
        if (IsKeyPressed(KEY_RIGHT) && !CheckCollision(currentTetromino.x + 1, currentTetromino.y))
            currentTetromino.x++;
        if (IsKeyPressed(KEY_DOWN))
            currentTetromino.y++;
        if (IsKeyPressed(KEY_UP))
            RotateTetromino();

        UpdateBoard();
        BeginDrawing();
        DrawBoard();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
