#include "display.h"

extern uint8_t digit1, digit2, digit3;

u32_t last_loop_time = 0;
u8_t currentDigit = 0;
bool dp1,dp2,dp3;

void updateBCDFromNumber(u8_t number, bool showDP = false) {

    digitalWrite(DISPLAY_D_PIN, LOW);
    digitalWrite(DISPLAY_C_PIN, LOW);
    digitalWrite(DISPLAY_B_PIN, LOW);
    digitalWrite(DISPLAY_A_PIN, LOW);
    digitalWrite(DISPLAY_DP_PIN, showDP);

    if (number >= 8) {
        digitalWrite(DISPLAY_D_PIN, HIGH);
        number = number - 8;
    }
    if (number >= 4) {
        digitalWrite(DISPLAY_C_PIN, HIGH);
        number = number - 4;
    }
    if (number >= 2) {
        digitalWrite(DISPLAY_B_PIN, HIGH);
        number = number - 2;
    }
    if (number >= 1) {
        digitalWrite(DISPLAY_A_PIN, HIGH);
        number = number - 1;
    }
}

void setupDisplay() {
    digitalWrite(DISPLAY_ENABLE_PIN, true);
    digitalWrite(DISPLAY_PWM_PIN, true);
}

void updateDisplayValue() {
    u32_t currentMillis = millis();

    if (currentMillis - last_loop_time >= DISPLAY_UPDATE_RATE) {
        digitalWrite(DISPLAY_ENABLE_PIN, false);
        switch (currentDigit) {
            case 0:
                digitalWrite(DISPLAY_1_ENABLE, true);
                digitalWrite(DISPLAY_2_ENABLE, false);
                digitalWrite(DISPLAY_3_ENABLE, false);
                updateBCDFromNumber(digit1,dp1);
                currentDigit++;
                break;

            case 1:
                digitalWrite(DISPLAY_1_ENABLE, false);
                digitalWrite(DISPLAY_2_ENABLE, true);
                digitalWrite(DISPLAY_3_ENABLE, false);
                updateBCDFromNumber(digit2,dp2);
                currentDigit++;
                break;

            case 2:
                digitalWrite(DISPLAY_1_ENABLE, false);
                digitalWrite(DISPLAY_2_ENABLE, false);
                digitalWrite(DISPLAY_3_ENABLE, true);
                updateBCDFromNumber(digit3,dp3);

                currentDigit = 0;
                break;

            default:
                currentDigit = 0;
                break;
        }
        digitalWrite(DISPLAY_ENABLE_PIN, true);
    }
}


void setDigits(u8_t (&digits)[3], bool (&dp)[3]){
    memcpy(&digit1,&digits[0],1);
    memcpy(&digit2,&digits[1],1);
    memcpy(&digit3,&digits[2],1);

    memcpy(&dp1,&dp[0],1);
    memcpy(&dp2,&dp[1],1);
    memcpy(&dp3,&dp[2],1);
}

void setFloat(float_t value,u8_t precision=2){
   int32_t number = (roundf(value * (10*precision)) / (10*precision) )*100;
    int32_t res = std::pow(10, (int)std::log10(number) + 1 - precision );
    digit1 =  (res / (int) pow(10, 0)) % 10;
    digit2 =  (res / (int) pow(10, 1)) % 10;
    digit3 =  (res / (int) pow(10, 2)) % 10;

    // ubicar el punto decimal:

}