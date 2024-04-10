#pragma once

extern "C" {
  #include "can2040.h"
}

extern struct can2040 cbus_wbo;

extern uint32_t currentO2;

// TODO: configurable por el pulsador/USB
static uint32_t stoich = 147000;