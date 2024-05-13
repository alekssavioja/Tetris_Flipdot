#include "GameLogic.h"

GameLogic::GameLogic()
    : grid{Grid()}, currentBlock{TetrominoBlock()}, nextBlock{TetrominoBlock()}, display{Display()}
{
    // Serial.println("In GAMELOGIC CONSTRUCT");
    currentBlock.setRandomShape();
    nextBlock.setRandomShape();
    LockedPiece = false;
    gameOver = false;
    lvl = 0;
    linesCleared = 0;
    prevlvl = 0;
}

void GameLogic::PlacePiece()
{
    grid.CleanGrid();
    currentBlock.Place(grid.grid);
}

void GameLogic::HandleInput(Controller &controller)
{
    byte Movement = 0;
    uint32_t parseButtons = 0;
    parseButtons = controller.getButtonStates();
    uint8_t button2 = (parseButtons & 0x000000FF);       // Least significant byte (buf[0])
    uint8_t button1 = (parseButtons & 0x0000FF00) >> 8;  // Second byte (buf[1])
    uint8_t button0 = (parseButtons & 0x00FF0000) >> 16; // Third byte (buf[2])

    // Handle button conditions
    if (button2 == 0x2F)
        Movement = 4; // CLICK A
    else if (button2 == 0x1F)
        Movement = 4; // Click B
    else if (button1 == 0xFF)
        Movement = 3; // Click DOWN
    else if (button0 == 0x00)
        Movement = 1; // CLICK LEFT
    else if (button0 == 0xFF)
        Movement = 2; // Click RIGHT

    switch (Movement)
    {
    case 1:
        MoveBlockLeft();
        break;
    case 2:
        MoveBlockRight();
        break;
    case 3:
        MoveBlockDown();
        break;
    case 4:
        RotateBlock();
        break;
    }

    PlacePiece();
    LockedPiece = false;
    RefreshDisplay();
    if (gameOver && Movement > 0)
    {
        gameOver = false;
        ResetGame();
    }
}

void GameLogic::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if (isBlockOutside() || (!isSpace()))
            currentBlock.UndoRotation();
    }
}

void GameLogic::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (isBlockOutside() || (!isSpace()))
            currentBlock.Move(0, 1);
    }
}

void GameLogic::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (isBlockOutside() || (!isSpace()))
            currentBlock.Move(0, -1);
    }
}

void GameLogic::MoveBlockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (isBlockOutside() || (!isSpace()))
        {
            currentBlock.Move(-1, 0);
            LockedPiece = true;
            PlacePiece(); // Update grid for lockedPieces
            LockBlock();
        }
    }
}

bool GameLogic::isBlockOutside()
{
    byte x = 0;
    byte y = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // Get the x and y positon of each cell of the block
            if ((currentBlock.shapePositions[currentBlock.currentRotation][i] & (1 << (3 - j))) > 0)
            {
                x = i + currentBlock.rowOffset;
                y = j + currentBlock.columnOffset;
            }
            // Checks if ROW is between 0-20 and COLUMN is between 0-10
            if (grid.isCellOutside(x, y))
            {
                return true;
            }
        }
    }
    return false;
}

bool GameLogic::isSpace()
{
    byte x = 0;
    byte y = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // Get the x and y positon of each cell of the block
            if ((currentBlock.shapePositions[currentBlock.currentRotation][i] & (1 << (3 - j))) > 0)
            {
                x = i + currentBlock.rowOffset;
                y = j + currentBlock.columnOffset;
            }
            // Checks lockedPieces[][] grid to see if there is a TetrominoBlock in that position
            if (grid.isCellEmpty(x, y) == false)
            {
                return false;
            }
        }
    }
    return true;
}

void GameLogic::LockBlock()
{
    // Saves the piece to lockedPieces Grid
    for (byte i = 0; i < grid.numRows; i++)
    {
        grid.lockedPieces[i] |= grid.grid[i];
    }

    memcpy(currentBlock.shapePositions, nextBlock.shapePositions, sizeof(currentBlock.shapePositions));
    currentBlock.currentRotation = nextBlock.currentRotation;
    currentBlock.columnOffset = nextBlock.columnOffset;
    currentBlock.rowOffset = nextBlock.rowOffset;

    if (!isSpace() && !display.endScreen)
    {
        gameOver = true;
        grid.CleanGrid();
        grid.CleanLockedPieces();
        display.GameOverText();
        display.DisplayData();
    }
    nextBlock.setRandomShape();

    linesCleared += grid.ClearFullRows();
    uint16_t currentlvl = (linesCleared / 10);

    if (currentlvl > prevlvl) 
    {
        prevlvl = currentlvl;
        IncreaseDifficulty();
    }
}

void GameLogic::ConvertDataForDisplay()
{
    uint16_t displayData[20] = {0};

    // Get the correct bit values of the current state of the game and place it into displayData[x] array as a 10 bitsized value
    for (byte row = 0; row < grid.numRows; row++)
    {
        displayData[row] |= (grid.grid[row] | grid.lockedPieces[row]);
    }

    byte nextBlockDATA[4] = {0};
    for (byte i = 0; i < 4; i++)
    {
        nextBlockDATA[i] |= nextBlock.shapePositions[nextBlock.currentRotation][i];
    }

    display.UpdateDisplay(displayData, nextBlockDATA);
    display.UpdateUIInfo(linesCleared, lvl);
}

void GameLogic::ResetGame()
{
    currentBlock.setRandomShape();
    nextBlock.setRandomShape();
    display.endScreen = false;

    grid.CleanGrid();
    grid.CleanLockedPieces();
}

void GameLogic::RefreshDisplay()
{
    if (!gameOver)
    {
        ConvertDataForDisplay();
        display.DisplayData();
    }
}

int GameLogic::IncreaseSpeed()
{
    return (800 - (lvl * 50));
}

void GameLogic::IncreaseDifficulty()
{
    lvl = (lvl < 9 ? lvl + 1 : 9);
}
