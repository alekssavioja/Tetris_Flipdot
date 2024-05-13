#pragma once
#include "Arduino.h"
#include "Usb.h"
#include "hiduniversal.h"

class Controller : public HIDReportParser
{
private:
    HIDUniversal hid;
    USB &usb;
    uint32_t buttonStates; 

    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);

public:
    Controller(USB &usb);
    void begin();  
    void update(); 
    uint32_t getButtonStates();
};