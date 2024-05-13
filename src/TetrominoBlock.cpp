#include "TetrominoBlock.h"
#include "TetrinoBlocks.h"

TetrominoBlock::TetrominoBlock()
    : currentRotation(0), rowOffset(0), columnOffset(3)
{
}

void TetrominoBlock::Rotate()
{
    currentRotation = (currentRotation + 1) % 4;
}

void TetrominoBlock::UndoRotation()
{
    if(currentRotation != 0) currentRotation--;
    else currentRotation = 3;
}

bool TetrominoBlock::Place(int (&grid)[20])
{
    for (int8_t row = 0; row < 4; row++)
    {
        for (int8_t column = 0; column < 4; column++)
        {       
            // Checks each cell of the TetrominoBlock and moves them to grid with the correct offset 
            if ( (shapePositions[currentRotation][row] & (1 << (3 - column)) ) >= 1)
            {
                grid[row + rowOffset] |= (1 << (9 - column - columnOffset));
            }
        }
    }
    return true;
}

void TetrominoBlock::Move(byte rows, byte columns)
{
    rowOffset += rows;
    columnOffset += columns;
}

void TetrominoBlock::setShape(Shapes shape)
{
    switch (shape)
    {
    case Shapes::L:
        memcpy_P(shapePositions, TetrominoBlocks::L, sizeof(shapePositions));
        break;
    case Shapes::J:
        memcpy_P(shapePositions, TetrominoBlocks::J, sizeof(shapePositions));
        break;
    case Shapes::S:
        memcpy_P(shapePositions, TetrominoBlocks::S, sizeof(shapePositions));
        break;
    case Shapes::Z:
        memcpy_P(shapePositions, TetrominoBlocks::Z, sizeof(shapePositions));
        break;
    case Shapes::I:
        memcpy_P(shapePositions, TetrominoBlocks::I, sizeof(shapePositions));
        break;
    case Shapes::O:
        memcpy_P(shapePositions, TetrominoBlocks::O, sizeof(shapePositions));
        break;
    case Shapes::T:
        memcpy_P(shapePositions, TetrominoBlocks::T, sizeof(shapePositions));
        break;
    default:
        return;
    }
}

void TetrominoBlock::setRandomShape()
{
    byte randomindex = random(0, 7);

    switch (randomindex)
    {
    case 0:
        setShape(TetrominoBlock::Shapes::L);
        rowOffset = 0;
        break;
    case 1:
        setShape(TetrominoBlock::Shapes::J);
        rowOffset = -1; // Adjusting where it spawns
        break;
    case 2:
        setShape(TetrominoBlock::Shapes::S);
        rowOffset = 0;
        break;
    case 3:
        setShape(TetrominoBlock::Shapes::Z);
        rowOffset = 0;
        break;
    case 4:
        setShape(TetrominoBlock::Shapes::I);
        rowOffset = -1; // Adjusting where it spawns
        break;
    case 5:
        setShape(TetrominoBlock::Shapes::O);
        rowOffset = 0;
        break;
    case 6:
    default:
        setShape(TetrominoBlock::Shapes::T);
        rowOffset = 0;
        break;
    }
}