#pragma once
#include "Arduino.h"

class Grid
{
private:
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, byte numRows);
    
public:
    Grid();

    byte numRows;       // 20
    byte numColumns;    // 10

    //byte grid[20][10]; // == 200 Bytes == 1600bit
    int grid[20];      // 0b 0000 0000 0000 0000
    int lockedPieces[20];
    
    void CleanGrid();
    void CleanLockedPieces();
    void PrintGrid();       // Debug
    bool isCellOutside(byte row, byte column);
    bool isCellEmpty(byte row, byte column);
    //void GameOverGrid();
    byte ClearFullRows();
};
