#include "Controller.h"
#include "Usb.h"
#include "hiduniversal.h"

Controller::Controller(USB &usb)
    : hid(&usb), usb(usb), buttonStates(0)
{
    hid.SetReportParser(0, this);
}

void Controller::begin()
{
}

void Controller::update()
{
    if (usb.getUsbTaskState() != USB_STATE_RUNNING)
    {
        //Serial.flush();
        Serial.println(("USB device not running."));
    }
}


void Controller::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{

    if (len < 8)
    {
        //Serial.println(F("HID report length too short, skipping."));
        return; 
    }
    // Serial.println(F("Parse Buffer is : "));


    uint32_t temp = 0;
    for (int i = 3; i < 6; ++i)
    {
        // Serial.print(buf[i], HEX);
        // Serial.print(" ");
        temp = (temp << 8) | buf[i];
    }

    buttonStates = temp;
}

uint32_t Controller::getButtonStates()
{
    return buttonStates;
}