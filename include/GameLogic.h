#pragma once
#include "Grid.h"
#include "TetrominoBlock.h"
#include "Controller.h"
#include "Display.h"

class GameLogic
{
private:
    Grid grid;
    TetrominoBlock currentBlock;
    TetrominoBlock nextBlock;
    bool LockedPiece;
    byte lvl;
    uint16_t prevlvl;
    uint16_t prevLines;

    void PlacePiece();
    void RotateBlock();
    void MoveBlockLeft();
    void MoveBlockRight();
    bool isBlockOutside();
    bool isSpace();
    void LockBlock();
    void ResetGame();
    void IncreaseDifficulty();

public:
    GameLogic();
    Display display;
    Controller controller;
    uint16_t linesCleared;
    bool gameOver;

    void HandleInput();
    void MoveBlockDown();
    void ConvertDataForDisplay();
    void RefreshDisplay();
    int IncreaseSpeed();
};
