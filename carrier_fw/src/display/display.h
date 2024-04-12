#pragma once
#include "main.h"
#include <cstdint>

extern uint8_t digit1,digit2,digit3,digit4;

#define DISPLAY_UPDATE_RATE 50
void setupDisplay();
void updateDisplayValue(); 
void updateBCDFromNumber(u8_t number);
void setSelectedDisplay(u8_t display_number);

#define DISPLAY_ENABLE_PIN 11
#define DISPLAY_PWM_PIN 19

#define DISPLAY_A_PIN  15
#define DISPLAY_B_PIN  16
#define DISPLAY_C_PIN  17
#define DISPLAY_D_PIN  18
#define DISPLAY_DP_PIN 20

#define DISPLAY_1_ENABLE 12
#define DISPLAY_2_ENABLE 13
#define DISPLAY_3_ENABLE 14