#pragma once
#include "pico/stdlib.h"
#include "../can/can.h"


struct wbo_configuration
{
    uint8_t sensorType; // LSU 4.9 for now (0x1 4.2 / 0x2 4.9 / 0x3 adv )
    uint8_t displayBrightness; // 0-255
    CAN_PROTOCOLS selectedCanInterface;
};


extern wbo_configuration wbo_cfg;
