#include "display.h"

extern uint8_t digit1, digit2, digit3;

u32_t last_loop_time = 0;
u8_t currentDigit = 0;

void updateBCDFromNumber(u8_t number)
{
}

void setupDisplay()
{
    digitalWrite(DISPLAY_ENABLE_PIN, true);
    digitalWrite(DISPLAY_PWM_PIN, true);
}

void updateDisplayValue()
{
    u32_t currentMillis = millis();

    if (currentMillis - last_loop_time >= DISPLAY_UPDATE_RATE)
    {
        digitalWrite(DISPLAY_ENABLE_PIN, false);
        switch (currentDigit)
        {
        case 0:
            digitalWrite(DISPLAY_1_ENABLE, true);
            digitalWrite(DISPLAY_2_ENABLE, false);
            digitalWrite(DISPLAY_3_ENABLE, false);
            updateBCDFromNumber(digit1);
            currentDigit++;
            break;

        case 1:
            digitalWrite(DISPLAY_1_ENABLE, false);
            digitalWrite(DISPLAY_2_ENABLE, true);
            digitalWrite(DISPLAY_3_ENABLE, false);
            updateBCDFromNumber(digit2);
            currentDigit++;
            break;

        case 2:
            digitalWrite(DISPLAY_1_ENABLE, false);
            digitalWrite(DISPLAY_2_ENABLE, false);
            digitalWrite(DISPLAY_3_ENABLE, true);
            updateBCDFromNumber(digit3);
            currentDigit++;
            break;

        default:
            currentDigit = 0;
            break;
        }
        digitalWrite(DISPLAY_ENABLE_PIN, true);
    }
}