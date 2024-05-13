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
    uint16_t linesCleared;
    bool gameOver;

    void PrintGame(); // Debug
    void HandleInput(Controller &controller);
    void MoveBlockDown();
    void ConvertDataForDisplay();
    void RefreshDisplay();
    int IncreaseSpeed();
};
