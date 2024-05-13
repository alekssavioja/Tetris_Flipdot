#pragma once
#include "Arduino.h"

class TetrominoBlock
{
private:

public:
    TetrominoBlock();

    enum Shapes { L, J, S, Z, I, O, T };

    byte currentRotation;
    int8_t rowOffset;
    int8_t columnOffset;
    byte shapePositions[4][4];
    
    bool Place(int (&grid)[20]);
    void Rotate();
    void UndoRotation();
    void Move(byte rows, byte columns);
    void setShape(Shapes shape);
    void setRandomShape();
};
