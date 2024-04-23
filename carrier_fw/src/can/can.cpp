#include "can.h"
#include "../configuration/configuration.h"

void sendWboStatus()
{
    switch (wbo_cfg.sensorType)
    {
    case CAN_PROTOCOLS::rusEFI:
        /* code */
        break;
    case CAN_PROTOCOLS::FTCan:
        break;
    case CAN_PROTOCOLS::AEMNet:
        break;
    case CAN_PROTOCOLS::HaltechCAN:
        break;
    default:
        break;
    }
}
