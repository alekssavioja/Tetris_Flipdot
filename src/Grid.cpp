#include "Grid.h"

Grid::Grid()
{
    numRows = 20;
    numColumns = 10;
    CleanGrid();
    CleanLockedPieces();
}

void Grid::CleanGrid()
{
    for (byte row = 0; row < numRows; row++)
    {
        grid[row] = 0;
    }
}

void Grid::CleanLockedPieces()
{
    for (byte row = 0; row < numRows; row++)
    {
        lockedPieces[row] = 0;
    }
}

void Grid::PrintGrid() // Debug
{
    for (byte row = 0; row < numRows; row++)
    {
        for (byte column = 0; column < numColumns; column++)
        {
            Serial.print(((grid[row] | lockedPieces[row]) & (1 << (numColumns - 1 - column))) ? "o" : ".");
            Serial.print(" ");
        }
        Serial.println();
    }
}

bool Grid::isCellOutside(byte row, byte column)
{
    if (row >= 0 && row < numRows && column >= 0 && column < numColumns)
    {
        return false;
    }
    return true;
}

bool Grid::isCellEmpty(byte row, byte column)
{
    return ((lockedPieces[row] & (1 << (numColumns - 1 - column))) == 0) ? true : false;
}

bool Grid::IsRowFull(int row)
{
    for (byte column = 0; column < numColumns; column++)
    {
        if ((lockedPieces[row] & (1 << column)) == 0)
        {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row)
{
    lockedPieces[row] = 0;
}

void Grid::MoveRowDown(int row, byte numRows)
{
    lockedPieces[row + numRows] = lockedPieces[row];
    lockedPieces[row] = 0;
}

byte Grid::ClearFullRows()
{
    byte fullRows = 0;
    for (int row = numRows - 1; row >= 0; row--)
    {
        if (IsRowFull(row))
        {
            ClearRow(row);
            fullRows++;
        }
        else if (fullRows > 0)
        {
            MoveRowDown(row, fullRows);
        }
    }
    return fullRows*6;
}