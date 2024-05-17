#include "GameLogic.h"
#include "Display.h"
#include "Controller.h"
#include <Arduino.h>
#include "Usb.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <usbhub.h>



unsigned long lastMoveTime = 0;
unsigned long lastInputReadTime = 0;
unsigned long lastInputProcessTime = 0;

const long inputReadInterval = 65;
unsigned long intervalForControls = 170;
unsigned long forcemoveDownInterval = 800;

GameLogic game = GameLogic();

void setup()
{
    int seed = analogRead(A0);
    seed ^= analogRead(A1) << 1;
    seed += analogRead(A2) << 2;
    randomSeed(seed);
    Serial.begin(57600);
    game.controller.begin();
}


void loop()
{
    unsigned long currentMillis = millis();

    // Layer 1: Input Reading
    if (currentMillis - lastInputReadTime >= inputReadInterval)
    {
        game.controller.update();
        lastInputReadTime = currentMillis;
    }

    // Layer 2: Input handeling
    if (currentMillis - lastInputProcessTime >= intervalForControls)
    {
        game.HandleInput();
        lastInputProcessTime = currentMillis;
    }

    // Layer 3: Forced Movement -- Game difficulty
    if (currentMillis - lastMoveTime >= forcemoveDownInterval)
    {
        game.MoveBlockDown();
        game.RefreshDisplay();
        forcemoveDownInterval = game.IncreaseSpeed();
        lastMoveTime = currentMillis;
    }
}
