#include "wideband_control.h"

// loop cada 0.5s? , wbo puede funcionar sin necesitar enviar el primer TX, actualiza cada 10mS
// https://github.com/mck1117/wideband/blob/bffc99c567758165da16b2db5406ac5410471e85/firmware/can.cpp#L44

int8_t msg_id = 0;
uint8_t data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t data2[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void wbo_module_loop()
{
    // valores a enviar:
    // ID: 0xEF50000
    // [0] => bateria en 0.1 => setear en 4, un boludo se olvido de agregar medicion de Vbatt en esta placa, sino traer desde can
    // [1] => motor en marcha/no (0/1), por ahora se setea luego de 5s de iniciado, si hay conexion por can revisar rpm/voltaje desde ahi
    can2040_msg out_msg;

    // en teoria hay que setear CAN2040_ID_EFF en el id tambien
    out_msg.id = 0xEF50000;
    int8_t msg[8] = {4, 1, 0, 0, 0, 0, 0};
    memcpy(out_msg.data, msg, 8);

    can2040_transmit(&cbus_wbo, &out_msg);
    Serial.printf("190--%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-\r\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    Serial.printf("191--%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-\r\n", data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6], data2[7]);
    if (data[1] = 0x1){
        Serial.printf("VALID LAMBDA: %d \r\n", currentO2);
    }
}

void wbo_module_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{

    // 190 ---
    //  a0,0,
    //  9b,27
    //  b,3,
    //  0,0,5

    // 191 ---
    // 2c,1,
    // 2e,0,
    // 0,0,75,0,5

    // 190
    //  -A0
    //  -0
    //  -74 -27
    //  -B0-30
    //  -00-00-

    // 191
    // -2C-01 esr
    // -5B-00 NernstDc
    // -00 PumpDuty
    // -00 Fault
    // -7D HeaterDuty
    // -00-
    // rusEFI Wbo module response
    if (msg->id == 0x190 || msg->id == 0x192 || true)
    {

        uint32_t inLambda;
        inLambda = (msg->data[3] << 8) | msg->data[2]; // Combining 2 bytes of data into single variable factor is 0.0001 so lambda 1 comes in as 10K
        if (msg->data[1] == 0x1)                       // Checking if lambda is valid
        {
            switch (msg->id)
            {
            case 0x190:
            case 0x192:
                if ((inLambda * stoich / 10000) > 250)
                { // Check if we dont overflow the 8bit O2 variable
                    currentO2 = 250;
                    break;
                }
                currentO2 = (unsigned int)(inLambda * stoich / 10000); // Multiplying lambda by stoich ratio to get AFR and dividing it by 10000 to get correct value
                break;
            default:
                break;
            }
        }
        if (msg->id == 0x190)
        {
            data[0] = msg->data[0];
            data[1] = msg->data[1];
            data[2] = msg->data[2];
            data[3] = msg->data[3];
            data[4] = msg->data[4];
            data[5] = msg->data[5];
            data[6] = msg->data[6];
            data[7] = msg->data[7];
        }
        if (msg->id == 0x191)
        {
            data2[0] = msg->data[0];
            data2[1] = msg->data[1];
            data2[2] = msg->data[2];
            data2[3] = msg->data[3];
            data2[4] = msg->data[4];
            data2[5] = msg->data[5];
            data2[6] = msg->data[6];
            data2[7] = msg->data[7];
        }
        // forward to external CAN
        sendRusEfiWbo(msg);
    }
}