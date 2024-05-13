#include "Display.h"

Display::Display() : 
                    dataScreen1{0x80, 0x83, 0x01, 
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x8F},
                    dataScreen2{0x80, 0x83, 0x02,
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
0x8F},
                    lineData1{0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F}, 
                    lineData2{0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F}, 
                    dataMask1(0x7F), dataMask2(0x70), UIMask(0x0F), numbers{0}, endScreen(0)
{
}

void Display::DisplayData()
{
    Serial.write(dataScreen1, 32);
    Serial.write(dataScreen2, 32);
}

// 1 == WHITE      0 == BLACK

void Display::UpdateDisplay(uint16_t newData[20], byte next[4])
{
    // Header :
    dataScreen1[0] = 0x80;
    dataScreen2[0] = 0x80;
    // Command :
    dataScreen1[1] = 0x83;
    dataScreen2[1] = 0x83;
    // Address :
    dataScreen1[2] = 0x01;
    dataScreen2[2] = 0x02;

    // Displaying the values set in UiUpdate
    for(byte i = 0; i < 8; i++) {
        dataScreen1[i+3] = lineData1[i];
        dataScreen2[i+3] = lineData2[i];
    }

    // Actual Game grid value reflection :
    for(byte i = 0; i < 20; i++){
        uint16_t temp = newData[i] >> 3;
        dataScreen1[i+11] = (temp & dataMask1);
        temp = newData[i] << 4;
        dataScreen2[i+11] &= 0x0F; // Set upper 4 bits to 0, while preserving the other 4 bit values
        dataScreen2[i+11] |= (temp & dataMask2);
    }

    // End bytes : 
    dataScreen1[31] = 0x8F;
    dataScreen2[31] = 0x8F;

    // Next Piece value reflection : 
    for(byte i = 0; i < 4; i++){
        dataScreen2[i+11] &= 0x70; // Set lower 4 bits to 0, while preserving the other 4 bit values
        dataScreen2[i+11] |= (next[i] & UIMask);
    }
    dataScreen2[15] &= 0xF0;
}

void Display::UpdateUIInfo(uint16_t lines, byte lv)
{
    // Extract the lvl and cleared lines data
    //long numbData = 0;
    //numbData = GetNumber(lv);
    byte temp[3] = {0};

    // Update line Data
    lineData1[0] = 0x0;
    lineData2[0] = 0x0;

    for (int8_t i = 2; i >= 0; i--) 
    {
        temp[i] = (lines % 10);
        lines /= 10;
    }

    for(uint8_t i = 0; i < 6; i++)
    {
        // Update lvl Data
        dataScreen2[i+24] &= 0x70;
        dataScreen2[i+24] |= (byte)((GetNumber(lv) >> (4*(5-i))) & UIMask);

        // Update line Data
        // 0110 1001 0010 0001 1001 0110
        //   15   11    7    3  <<1  <<5    i == 4 ja i == 5
        lineData1[i+1] &= temp[0] > 0 ? 0x07 : 0x7F;
        lineData1[i+1] |= temp[0] > 0 ? (byte)((GetNumber(temp[0]) >> ((4*(5-i))-3)) & 0x78) : 0x0;

        lineData2[i+1] &= temp[2] > 0 ? 0x70 : 0x7F;
        lineData2[i+1] |= temp[2] > 0 ? (byte)((GetNumber(temp[2]) >> ( 4*(5-i)) )   & 0x0F) : 0x7F;   

        lineData1[i+1] &= temp[1] > 0 ? 0x78 : 0x7F;
        lineData1[i+1] |= temp[1] > 0 ? (byte)((GetNumber(temp[1]) >> ((4*(5-i))+2)) & 0x3)  : 0x0;

        lineData2[i+1] &= temp[1] > 0 ? 0x0F : 0x7F;
        if(i <= 3)
        {
            lineData2[i+1] |= temp[1] > 0 ? (byte)((GetNumber(temp[1]) >> ( ( 4*(4-i) ) -1 )) & 0x60)  : 0x0;
        } else if (i == 4) {
            lineData2[i+1] |= temp[1] > 0 ? (byte)((GetNumber(temp[1]) << 1) & 0x60)  : 0x0;
        } else {
            lineData2[i+1] |= temp[1] > 0 ? (byte)((GetNumber(temp[1]) << 5) & 0x60)  : 0x0;
        }
        
    }

    lineData1[7] = 0x7F;
    lineData2[7] = 0x7F;
}

/*
lineData1[i+1] = numbers[0] > 0 ? (byte)((GetNumber(numbers[0]) >> ((4*(5-i))-3)) & 0x78) : 0x7F;
lineData2[i+1] = numbers[2] > 0 ? (byte)((GetNumber(numbers[2]) >> ( 4*(5-i)))     & 0x0F) : 0x7F;   

lineData1[i+1] = numbers[1] > 0 ? (byte)((GetNumber(numbers[1]) >> ((4*(5-i))+3)) & 0x3)  : 0x7F;
lineData2[i+1] = numbers[1] > 0 ? (byte)((GetNumber(numbers[1]) >> ((4*(5-i))-3)) & 0x96) : 0x7F;
*/

void Display::ClearDisplayData()
{
    for(byte i = 0; i < 28; i++)
    {
        dataScreen1[i+3] = 0;
        dataScreen2[i+3] = 0;
    }
}

void Display::GameOverText()
{
    ClearDisplayData();
    // GA
    dataScreen1[4] |= (0b0001110 & dataMask1);
    dataScreen2[4] |= (0b0011000 & dataMask1);

    dataScreen1[5] |= (0b0010000 & dataMask1);
    dataScreen2[5] |= (0b0100100 & dataMask1);

    dataScreen1[6] |= (0b0010110 & dataMask1);
    dataScreen2[6] |= (0b0100100 & dataMask1);

    dataScreen1[7] |= (0b0010010 & dataMask1);
    dataScreen2[7] |= (0b0111100 & dataMask1);

    dataScreen1[8] |= (0b0001110 & dataMask1);
    dataScreen2[8] |= (0b0100100 & dataMask1);
    // ME
    dataScreen1[10] |= (0b0010001 & dataMask1);
    dataScreen2[10] |= (0b0011100 & dataMask1);

    dataScreen1[11] |= (0b0011011 & dataMask1);
    dataScreen2[11] |= (0b0010000 & dataMask1);

    dataScreen1[12] |= (0b0010101 & dataMask1);
    dataScreen2[12] |= (0b0011000 & dataMask1);

    dataScreen1[13] |= (0b0010001 & dataMask1);
    dataScreen2[13] |= (0b0010000 & dataMask1);

    dataScreen1[14] |= (0b0010001 & dataMask1);
    dataScreen2[14] |= (0b0011100 & dataMask1);

    dataScreen1[16] |= (0b1111111 & dataMask1);
    dataScreen2[16] |= (0b1111111 & dataMask1);
    // OV
    dataScreen1[18] |= (0b0011100 & dataMask1);
    dataScreen2[18] |= (0b0100010 & dataMask1);

    dataScreen1[19] |= (0b0100010 & dataMask1);
    dataScreen2[19] |= (0b0100010 & dataMask1);

    dataScreen1[20] |= (0b0100010 & dataMask1);
    dataScreen2[20] |= (0b0100010 & dataMask1);

    dataScreen1[21] |= (0b0100010 & dataMask1);
    dataScreen2[21] |= (0b0010100 & dataMask1);

    dataScreen1[22] |= (0b0011100 & dataMask1);
    dataScreen2[22] |= (0b0001000 & dataMask1);
    // ER
    dataScreen1[24] |= (0b0011100 & dataMask1);
    dataScreen2[24] |= (0b0111000 & dataMask1);

    dataScreen1[25] |= (0b0010000 & dataMask1);
    dataScreen2[25] |= (0b0100100 & dataMask1);

    dataScreen1[26] |= (0b0011000 & dataMask1);
    dataScreen2[26] |= (0b0111000 & dataMask1);

    dataScreen1[27] |= (0b0010000 & dataMask1);
    dataScreen2[27] |= (0b0100100 & dataMask1);

    dataScreen1[28] |= (0b0011100 & dataMask1);
    dataScreen2[28] |= (0b0100100 & dataMask1);

    endScreen = true;
}

long Display::GetNumber(byte request)
{
    switch (request)
    {
    case 0:
        return 0b011010011011110110010110;  // 0110 1001 1011 1101 1001 0110
        break;
    case 1:
        return 0b001001100010001000100111;  // 0010 0110 0010 0010 0010 0111
        break;
    case 2:
        return 0b011010010001001001001111;  // 0110 1001 0001 0010 0100 1111
        break;
    case 3:
        return 0b011010010010000110010110;  // 0110 1001 0010 0001 1001 0110
        break;
    case 4:
        return 0b100110011111000100010001;  // 1001 1001 1111 0001 0001 0001
        break;
    case 5:
        return 0b111110001110000100011110;  // 1111 1000 1110 0001 0001 1110
        break;
    case 6:
        return 0b011110001110100110010110;  // 0111 1000 1110 1001 1001 0110
        break;
    case 7:
        return 0b011100010010011100100010;  // 0111 0001 0010 0111 0010 0010
        break;
    case 8:
        return 0b011010010110100110010110;  // 0110 1001 0110 1001 1001 0110
        break;
    case 9:
        return 0b011110011111000100010111;  // 0111 1001 1111 0001 0001 0111
        break;
    default:
        return 0b011010011011110110010110;
        break;
    }
}

/*
Next Block on row 8-12
L       on    row 14-16
V       on    row 18-20
Num     on    row 22-27
*/

/*
Numbers
0 == [1]: 110   ,[2]: 1001  ,[3]: 1011  ,[4]: 1101  ,[5]: 1001  ,[6]: 110
1 == [1]: 10    ,[2]: 110   ,[3]: 10    ,[4]: 10    ,[5]: 10    ,[6]: 111
2 == [1]: 110   ,[2]: 1001  ,[3]: 1     ,[4]: 10    ,[5]: 100   ,[6]: 1111
3 == [1]: 110   ,[2]: 1001  ,[3]: 10    ,[4]: 1     ,[5]: 1001  ,[6]: 110
4 == [1]: 1001  ,[2]: 1001  ,[3]: 1111  ,[4]: 1     ,[5]: 1     ,[6]: 1
5 == [1]: 1111  ,[2]: 1000  ,[3]: 1110  ,[4]: 1     ,[5]: 1     ,[6]: 1110
6 == [1]: 111   ,[2]: 1000  ,[3]: 1110  ,[4]: 1001  ,[5]: 1001  ,[6]: 110
7 == [1]: 111   ,[2]: 1     ,[3]: 10    ,[4]: 111   ,[5]: 10    ,[6]: 10
8 == [1]: 110   ,[2]: 1001  ,[3]: 110   ,[4]: 1001  ,[5]: 1001  ,[6]: 110
9 == [1]: 111   ,[2]: 1001  ,[3]: 1111  ,[4]: 1     ,[5]: 1     ,[6]: 111
*/

/*
Data BIT HIGH == White
Data BIT LOW  == Black

dataScreen1{0x80, 0x83, 0x00, // Commands and Address
0x80, 0x5D, 0x44, 0x4C, 0x44, 0x6E, 0x66, 0x7F, 
0x00, // [0] = 11
0x00, // [1] = 12
0x00, // [2] = 13
0x00, // [3] = 14
0x00, // [4] = 15
0x00, // [5] = 16
0x00, // [6] = 17
0x00, // [7] = 18
0x00, // [8] = 19
0x00, // [9] = 20
0x00, // [10] = 21
0x00, // [11] = 22
0x00, // [12] = 23
0x00, // [13] = 24
0x00, // [14] = 25
0x00, // [15] = 26
0x00, // [16] = 27
0x00, // [17] = 28
0x00, // [18] = 29
0x00, // [19] = 30
0x8F}; // End byte


dataScreen2{0x80, 0x83, 0x01, // Commands and Address
0x77, 0x7F, 0x34, 0x53, 0x30, 0x5E, 0x59, 0x7F,
0x00, // [0] = 11
0x00, // [1] = 12
0x00, // [2] = 13
0x00, // [3] = 14
0x00, // [4] = 15
0x00, // [5] = 16
0x00, // [6] = 17
0x00, // [7] = 18
0x00, // [8] = 19
0x00, // [9] = 20
0x00, // [10] = 21
0x00, // [11] = 22
0x00, // [12] = 23
0x00, // [13] = 24
0x00, // [14] = 25
0x00, // [15] = 26
0x00, // [16] = 27
0x00, // [17] = 28
0x00, // [18] = 29
0x00, // [19] = 30
0x8F};  // End byte
*/

/*       
// GA
grid[1][j] = (0b0001110 0011000 & (1 << (13 - j))) ? 1 : 0;
grid[2][j] = (0b0010000 0100100 & (1 << (13 - j))) ? 1 : 0;
grid[3][j] = (0b0010110 0100100 & (1 << (13 - j))) ? 1 : 0;
grid[4][j] = (0b0010010 0111100 & (1 << (13 - j))) ? 1 : 0;
grid[5][j] = (0b0001110 0100100 & (1 << (13 - j))) ? 1 : 0;
// ME
grid[7][j] = (0b0010001 0011100 & (1 << (13 - j))) ? 1 : 0;
grid[8][j] = (0b0011011 0010000 & (1 << (13 - j))) ? 1 : 0;
grid[9][j] = (0b0010101 0011000 & (1 << (13 - j))) ? 1 : 0;
grid[10][j] = (0b0010001 0010000 & (1 << (13 - j))) ? 1 : 0;
grid[11][j] = (0b0010001 0011100 & (1 << (13 - j))) ? 1 : 0;
grid[13][j] = (0b1111111 1111111 & (1 << (13 - j))) ? 1 : 0;
// OV
grid[15][j] = (0b0011100 0100010 & (1 << (13 - j))) ? 1 : 0;
grid[16][j] = (0b0100010 0100010 & (1 << (13 - j))) ? 1 : 0;
grid[17][j] = (0b0100010 0100010 & (1 << (13 - j))) ? 1 : 0;
grid[18][j] = (0b0100010 0010100 & (1 << (13 - j))) ? 1 : 0;
grid[19][j] = (0b0011100 0001000 & (1 << (13 - j))) ? 1 : 0;
// ER
grid[21][j] = (0b0011100 0111000 & (1 << (13 - j))) ? 1 : 0;
grid[22][j] = (0b0010000 0100100 & (1 << (13 - j))) ? 1 : 0;
grid[23][j] = (0b0011000 0111000 & (1 << (13 - j))) ? 1 : 0;
grid[24][j] = (0b0010000 0100100 & (1 << (13 - j))) ? 1 : 0;
grid[25][j] = (0b0011100 0100100 & (1 << (13 - j))) ? 1 : 0;
*/