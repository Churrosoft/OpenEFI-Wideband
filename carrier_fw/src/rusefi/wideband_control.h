#ifndef WIDEBAND_CONTROL_H
#define WIDEBAND_CONTROL_H

#include "pico/stdlib.h"
#include "math.h"
#include "main.h"

extern "C" {
  #include "can2040.h"
  #include <string.h>

}

void wbo_module_loop();

void wbo_module_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg);

#endif