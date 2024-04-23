#ifndef EXT_CAN_H
#define EXT_CAN_H
#include "main.h"

void sendWboStatus();

enum CAN_PROTOCOLS : uint8_t
{
    rusEFI = 0x4,
    FTCan,
    AEMNet,
    HaltechCAN
};

#endif