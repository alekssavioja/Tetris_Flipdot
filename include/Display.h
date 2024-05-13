#pragma once
#include"Arduino.h"

class Display
{
private:
    byte dataScreen1[32];
    byte dataScreen2[32];
    byte lineData1[8];
    byte lineData2[8];
    byte dataMask1; // 0x7F For Gamefield
    byte dataMask2; // 0x70 For Gamefield
    byte UIMask;    // 0x0F
    
    long GetNumber(byte request);
    void ClearDisplayData();   

public:
    Display();
    bool endScreen;
    void UpdateDisplay(uint16_t newData[20], byte next[4]);
    void UpdateUIInfo(uint16_t lines, byte lv);
    void DisplayData();
    void GameOverText();
};
