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


    int grid[20];      // 0b 0000 0000 0000 0000
    int lockedPieces[20];
    
    void CleanGrid();
    void CleanLockedPieces();
    void PrintGrid();       // Debug
    bool isCellOutside(byte row, byte column);
    bool isCellEmpty(byte row, byte column);
    byte ClearFullRows();
};
