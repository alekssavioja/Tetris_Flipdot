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
                    dataMask1(0x7F), dataMask2(0x70), UIMask(0x0F), endScreen(0)
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
    dataScreen2[12] &= 0xF0;
    for(byte i = 0; i < 4; i++){
        dataScreen2[i+13] &= 0x70; // Set lower 4 bits to 0, while preserving the other 4 bit values
        dataScreen2[i+13] |= (next[i] & UIMask);
    }
    //dataScreen2[17] &= 0xF0;
}

void Display::UpdateUIInfo(uint16_t lines, byte lv)
{
    // Extract the lvl and cleared lines data
    byte nhundreds = ((lines / 100) % 10);
    byte ntens = ((lines / 10) % 10);
    byte nsingles = ((lines / 1) % 10);

    // Update line Data + lvl data
    if (lines < 10) {
        for(uint8_t i = 0; i < 5; i++)
        {
            dataScreen2[i+24] &= 0x70;
            dataScreen2[i+24] |= (byte)((GetNumber(lv) >> (4*(4-i))) & UIMask);

            lineData2[i+1] &= 0x60;
            lineData2[i+1] |= (byte)((GetNumber(nsingles) >> (4*(4-i))) & 0x0F);

            lineData2[0] &= 0x60;
            lineData2[6] &= 0x60;
        }
    } else if (lines >= 10 && lines < 100) {
        for(uint8_t i = 0; i < 5; i++)
        {
            dataScreen2[i+24] &= 0x70;
            dataScreen2[i+24] |= (byte)((GetNumber(lv) >> (4*(4-i))) & UIMask);

            lineData2[i+1] &= 0x60;
            lineData2[i+1] |= (byte)((GetNumber(nsingles) >> (4*(4-i))) & 0x0F);

            lineData1[i+1] &= 0x78;
            lineData1[i+1] |= (byte)((GetNumber(ntens) >> ((4*(4-i))+2)) & 0x3);

            lineData2[i+1] &= 0x0F;
            lineData2[i+1] |= (byte)(((GetNumber(ntens) << 5 ) >> ( 4*(4-i))) & 0x60);
            
            lineData2[0] &= 0x0;
            lineData1[0] &= 0x78;
            lineData2[6] &= 0x0;
            lineData1[6] &= 0x78;
        }
    } else {
        for(uint8_t i = 0; i < 5; i++)
        {
            // Update lvl Data
            dataScreen2[i+24] &= 0x70;
            dataScreen2[i+24] |= (byte)((GetNumber(lv) >> (4*(4-i))) & UIMask);

            lineData1[i+1] &= 0x07;
            lineData1[i+1] |= (byte)(((GetNumber(nhundreds) << 3) >> (4*(4-i)) ) & 0x78);

            lineData2[i+1] &= 0x60;
            lineData2[i+1] |= (byte)((GetNumber(nsingles) >> (4*(4-i))) & 0x0F);

            lineData1[i+1] &= 0x78;
            lineData1[i+1] |= (byte)((GetNumber(ntens) >> ((4*(4-i))+2)) & 0x3);
            // 1001 1001 1111 0001 0001 0000 0
            lineData2[i+1] &= 0x0F;
            lineData2[i+1] |= (byte)(((GetNumber(ntens) << 5 ) >> ( 4*(4-i))) & 0x60);

            lineData2[0] &= 0x0;
            lineData1[0] &= 0x0;
            lineData2[6] &= 0x0;
            lineData1[6] &= 0x0;
        }
    }
    dataScreen2[23] &= 0x70;
    dataScreen2[29] &= 0x70;
    lineData1[7] = 0x7F;
    lineData2[7] = 0x7F;
}

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
        return 0b01101011110110010110;  // 0110 1011 1101 1001 0110
        break;
    case 1:
        return 0b00100110001000100111;  // 0010 0110 0010 0010 0111
        break;
    case 2:
        return 0b01101001001001001111;  // 0110 1001 0010 0100 1111
        break;
    case 3:
        return 0b11100001011000011110;  // 1110 0001 0110 0001 1110
        break;
    case 4:
        return 0b10011001111100010001;  // 1001 1001 1111 0001 0001
        break;
    case 5:
        return 0b11111000111000011110;  // 1111 1000 1110 0001 1110
        break;
    case 6:
        return 0b01101000111010010110;  // 0110 1000 1110 1001 0110
        break;
    case 7:
        return 0b11110001001000100010;  // 1111 0001 0010 0010 0010
        break;
    case 8:
        return 0b01101001011010010110;  // 0110 1001 0110 1001 0110
        break;
    case 9:
        return 0b11111001111100011111;  // 1111 1001 1111 0001 1111
        break;
    default:
        return 0b01101011110110010110;
        break;
    }
}

void Display::ResetData()
{
    byte tempDataScreen1[32] = {
        0x80, 0x83, 0x01, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F
    };

    byte tempDataScreen2[32] = {
        0x80, 0x83, 0x02, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x8F
    };

    memcpy(dataScreen1, tempDataScreen1, sizeof(dataScreen1));
    memcpy(dataScreen2, tempDataScreen2, sizeof(dataScreen2));

    for(byte i = 0; i < 8; i++)
    {
        lineData1[i] = 0x7F;
        lineData2[i] = 0x7F;
    }
}

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
        return 0b111110011111000100011111;  // 1111 1001 1111 0001 0001 1111
        break;
    default:
        return 0b011010011011110110010110;
        break;
    }
}
*/
/*
    for(uint8_t i = 0; i < 6; i++)
    {
        // Update lvl Data
        dataScreen2[i+23] &= 0x70;
        dataScreen2[i+23] |= (byte)((GetNumber(lv) >> (4*(5-i))) & UIMask);

        // Update line Data
        // 0110 1001 0010 0001 1001 0110
        //   15   11    7    3  <<1  <<5
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
    */