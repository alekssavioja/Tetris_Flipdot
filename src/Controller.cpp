#include "Controller.h"
#include "Usb.h"
#include "hiduniversal.h"

Controller::Controller()
    :  usb(), hid(&usb), buttonStates(0)
{
    hid.SetReportParser(0, this);
}

void Controller::begin()
{
    if (usb.Init() == -1) {
        Serial.println("USB Host did not start.");
        while (1); // Halt if USB host initialization fails
    }
    Serial.println("USB Host initialized.");
}

void Controller::update()
{
    usb.Task();

    if (usb.getUsbTaskState() != USB_STATE_RUNNING)
    {
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

    uint32_t temp = 0;
    for (int i = 3; i < 6; ++i)
    {
        temp = (temp << 8) | buf[i];
    }

    buttonStates = temp;
}

uint32_t Controller::getButtonStates()
{
    return buttonStates;
}