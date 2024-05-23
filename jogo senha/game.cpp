#include <iostream>
#include "game.h"
#include <raylib.h>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <string>
#define TRANSPARENT {0, 0, 0, 0}
bool inGame = false;
int selectedColor = 0;
int answer[4];

int arr[4] = { 0, 0, 0, 0 };
int colorMatrix[10][4] =
{
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7},
    {7, 7, 7, 7}
};

int correctionMatrix[10][4] =
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int tries = 0;
bool gameOver = false;
bool gameWon = false;

using namespace std;
Game::Game()
{

}

Color colors[] = { RED, ORANGE, YELLOW, PINK, DARKPURPLE, BLUE, GREEN };

void RandomColors() {
    if (!inGame) {
        for (int i = 0; i < 4; i++) {
            answer[i] = rand() % 7;
        }
        inGame = true;
    }
}

void Game::DrawCorrection() {
    int offsetX = GetScreenWidth() / 7;
    int offsetY = 25;
    int gridRows = 10;
    const int gridCols = 5;
    int availableHeight = GetScreenHeight() - 2 * offsetY - 55;
    int cellSize = std::min((GetScreenWidth() - 2 * offsetX) / gridCols, availableHeight / gridRows);

    Color colorLookup[] = { TRANSPARENT, BLACK, WHITE };

    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {
            int centerX = offsetX + col * cellSize + cellSize / 2;
            int centerY = offsetY + row * cellSize + cellSize / 2;

            if (row < tries && col == gridCols - 1) {
                int smallCircleSize = cellSize / 6;
                DrawCircle(centerX - smallCircleSize * 1.5, centerY - smallCircleSize * 1.5, smallCircleSize, colorLookup[correctionMatrix[row][0]]);
                DrawCircle(centerX + smallCircleSize * 1.5, centerY - smallCircleSize * 1.5, smallCircleSize, colorLookup[correctionMatrix[row][1]]);
                DrawCircle(centerX - smallCircleSize * 1.5, centerY + smallCircleSize * 1.5, smallCircleSize, colorLookup[correctionMatrix[row][2]]);
                DrawCircle(centerX + smallCircleSize * 1.5, centerY + smallCircleSize * 1.5, smallCircleSize, colorLookup[correctionMatrix[row][3]]);
            }
        }
    }

    bool usedAnswer[4] = { false };

    for (int col = 0; col < gridCols - 1; ++col) {
        if (colorMatrix[tries][col] == answer[col]) {
            arr[col] = 1;
            usedAnswer[col] = true;
        }
    }

    for (int col = 0; col < gridCols - 1; ++col) {
        if (arr[col] != 1) {
            for (int ansCol = 0; ansCol < gridCols - 1; ++ansCol) {
                if (!usedAnswer[ansCol] && colorMatrix[tries][col] == answer[ansCol]) {
                    arr[col] = 2;
                    usedAnswer[ansCol] = true;
                    break;
                }
            }
        }
    }

    std::sort(arr, arr + 4);

    correctionMatrix[tries][0] = arr[3];
    correctionMatrix[tries][1] = arr[2];
    correctionMatrix[tries][2] = arr[1];
    correctionMatrix[tries][3] = arr[0];

    std::fill(std::begin(arr), std::end(arr), 0);
}

void Game::ColorPicker() {
    int rectSize = 60;
    int numColors = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < numColors; ++i) {
        DrawRectangle(rectSize * i, 640, rectSize, rectSize, colors[i]);
        DrawRectangleLines(rectSize * i, 640, rectSize, rectSize, BLACK);
    }

    DrawLine(0, 639, GetScreenWidth(), 639, BLACK);

    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    for (int i = 0; i < numColors; ++i) {
        if (mouseX > rectSize * i && mouseX < rectSize * (i + 1) && mouseY > 640 && mouseY < 640 + rectSize) {
            DrawRectangle(rectSize * i, 640, rectSize, rectSize, colors[i]);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedColor = i;
            }
        }
    }
    DrawRectangle(rectSize * selectedColor, 690, rectSize, rectSize, BLACK);
}

void Game::DrawInterface() {
    int offsetX = GetScreenWidth() / 7;
    int offsetY = 25;
    int gridRows = 10;
    int gridCols = 5;
    int availableHeight = GetScreenHeight() - 2 * offsetY - 55;
    int cellSize = std::min((GetScreenWidth() - 2 * offsetX) / gridCols, availableHeight / gridRows);

    for (int row = 0; row <= gridRows; ++row) {
        int y = offsetY + row * cellSize;
        DrawLine(offsetX, y, offsetX + gridCols * cellSize, y, BLACK);

        for (int col = 0; col <= gridCols; ++col) {
            if (col == 0 || col == 4 || col == 5) {
                int x = offsetX + col * cellSize;
                DrawLine(x, offsetY, x, offsetY + gridRows * cellSize, BLACK);
            }

            int centerX = offsetX + col * cellSize + cellSize / 2;
            int centerY = offsetY + row * cellSize + cellSize / 2;

            if (row <= gridRows - 1 && col == gridCols - 1) {
                int smallCircleSize = cellSize / 6;
                DrawCircleLines(centerX - smallCircleSize * 1.5, centerY - smallCircleSize * 1.5, smallCircleSize, BLACK);
                DrawCircleLines(centerX + smallCircleSize * 1.5, centerY - smallCircleSize * 1.5, smallCircleSize, BLACK);
                DrawCircleLines(centerX - smallCircleSize * 1.5, centerY + smallCircleSize * 1.5, smallCircleSize, BLACK);
                DrawCircleLines(centerX + smallCircleSize * 1.5, centerY + smallCircleSize * 1.5, smallCircleSize, BLACK);
            }
            else if (col != 4 && col != 5 && row != 10) {
                DrawCircleLines(centerX, centerY, cellSize / 2.4, BLACK);
            }
        }
    }

    DrawRectangle(offsetX - 3, offsetY + cellSize * tries, 3, cellSize, BLACK);
}

void Game::DrawColors() {
    int offsetX = GetScreenWidth() / 7;
    int offsetY = 25;
    int gridRows = 10;
    int gridCols = 5;
    int availableHeight = GetScreenHeight() - 2 * offsetY - 55;
    int cellSize = std::min((GetScreenWidth() - 2 * offsetX) / gridCols, availableHeight / gridRows);
    float radius = cellSize / 2.4;

    Color colorLookup[] = { RED, ORANGE, YELLOW, PINK, DARKPURPLE, BLUE, GREEN, TRANSPARENT };

    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols - 1; ++col) {
            int centerX = offsetX + col * cellSize + cellSize / 2;
            int centerY = offsetY + row * cellSize + cellSize / 2;

            int colorIndex = colorMatrix[row][col];
            if (colorIndex >= 0 && colorIndex < sizeof(colorLookup) / sizeof(colorLookup[0])) {
                DrawCircle(centerX, centerY, radius, colorLookup[colorIndex]);

                int distanceX = GetMouseX() - centerX;
                int distanceY = GetMouseY() - centerY;
                int distance = sqrt(distanceX * distanceX + distanceY * distanceY);

                if (distance <= radius) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        colorMatrix[tries][col] = selectedColor;
                    }
                    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                        colorMatrix[tries][col] = 7;
                    }
                }
            }
        }
    }
}

void Game::Run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        RandomColors();
        ClearBackground(DARKGRAY);
            Color colorLookup[] = { RED, ORANGE, YELLOW, PINK, DARKPURPLE, BLUE, GREEN, TRANSPARENT };
        if (!gameOver && !gameWon) {
            ColorPicker();
            DrawInterface();
            DrawColors();
            DrawCorrection();

            if (colorMatrix[tries][0] != 7 && colorMatrix[tries][1] != 7 && colorMatrix[tries][2] != 7 && colorMatrix[tries][3] != 7) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    tries++;
                    bool win = true;
                    for (int i = 0; i < 4; ++i) {
                        if (colorMatrix[tries - 1][i] != answer[i]) {
                            win = false;
                            break;
                        }
                    }
                    if (win) {
                        gameWon = true;
                    }
                    else if (tries >= 10) {
                        gameOver = true;
                    }
                }
            }
        }
        else {
            if (gameOver) {
                DrawText("Game Over!", GetScreenWidth() / 2 - MeasureText("Game Over!", 40) / 2, GetScreenHeight() / 2 - 50, 40, RED);
                
            }
            else if (gameWon) {
                DrawText("You Win!", GetScreenWidth() / 2 - MeasureText("You Win!", 40) / 2, GetScreenHeight() / 2 - 50, 40, GREEN);
                
            }

            // Desenha as cores correspondentes aos números na matriz answer
            int offsetX = GetScreenWidth() / 2 - 100;
            int offsetY = GetScreenHeight() / 2;
            int spacing = 50;

            for (int i = 0; i < 4; ++i) {
                DrawRectangle(offsetX + i * spacing, offsetY, 40, 40, colors[answer[i]]);
                DrawRectangleLines(offsetX + i * spacing, offsetY, 40, 40, BLACK);
            }


            for (int row = 0; row < 10; ++row) {
                for (int col = 0; col < 4; ++col) {
                    int centerX = GetScreenWidth() / 2 - 50 + col * 25 + 25 / 2;
                    int centerY = GetScreenHeight() / 22 + row * 25 + 25 / 2;

                    int colorIndex = colorMatrix[row][col];
                    if (colorIndex >= 0 && colorIndex < sizeof(colorLookup) / sizeof(colorLookup[0])) {
                        DrawCircle(centerX, centerY, 10, colorLookup[colorIndex]);



                    }
                }
            }

            std::string attemptsText = "Attempts: " + std::to_string(tries);
            DrawText(attemptsText.c_str(), GetScreenWidth() / 2 - MeasureText(attemptsText.c_str(), 20) / 2, GetScreenHeight() / 2 + 50, 20, WHITE);

            


            DrawText("Press R to Restart", GetScreenWidth() / 2 - MeasureText("Press R to Restart", 20) / 2, GetScreenHeight() / 2 + 80, 20, WHITE);

            if (IsKeyPressed(KEY_R)) {
                gameOver = false;
                gameWon = false;
                inGame = false;
                tries = 0;
                for (int row = 0; row < 10; ++row) {
                    for (int col = 0; col < 4; ++col) {
                        colorMatrix[row][col] = 7;
                        correctionMatrix[row][col] = 0;
                    }
                }
                std::fill(std::begin(arr), std::end(arr), 0);
                RandomColors();
            }
        }

        if (IsKeyPressed(KEY_ONE)) selectedColor = 0;
        if (IsKeyPressed(KEY_TWO)) selectedColor = 1;
        if (IsKeyPressed(KEY_THREE)) selectedColor = 2;
        if (IsKeyPressed(KEY_FOUR)) selectedColor = 3;
        if (IsKeyPressed(KEY_FIVE)) selectedColor = 4;
        if (IsKeyPressed(KEY_SIX)) selectedColor = 5;
        if (IsKeyPressed(KEY_SEVEN)) selectedColor = 6;

        EndDrawing();
    }
}
