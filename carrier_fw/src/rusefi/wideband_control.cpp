#include "wideband_control.h"

// loop cada 0.5s? para pedir info sobre la wbo
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
}

void wbo_module_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // rusEFI Wbo module response
    if (msg->id == 0x190 || msg->id == 0x192)
    {

        uint32_t inLambda;
        inLambda = (msg->data[3] << 8) | msg->data[2]; // Combining 2 bytes of data into single variable factor is 0.0001 so lambda 1 comes in as 10K
        if (msg->data[1] == 0x1)                       // Checking if lambda is valid
        {
            switch (msg->id)
            {
            case 0x190:
                if ((inLambda * stoich / 10000) > 250)
                { // Check if we dont overflow the 8bit O2 variable
                    currentO2 = 250;
                    break;
                }
                currentO2 = (unsigned int)(inLambda * stoich / 10000); // Multiplying lambda by stoich ratio to get AFR and dividing it by 10000 to get correct value
                break;

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
    }
}