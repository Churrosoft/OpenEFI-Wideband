#include "main.h"
#include "rusefi/wideband_control.h"
#include "display/display.h"

struct can2040 cbus_wbo;
uint32_t currentO2 = 0;
static struct can2040 cbus_external;

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus_wbo);
}

static void PIOx_IRQHandler2(void)
{
    can2040_pio_irq_handler(&cbus_external);
}

static void cbus_ext_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Add message processing code here...
}

void canbus_setup(void)
{
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus (rusEFI WBO module)
    can2040_setup(&cbus_wbo, 0);
    can2040_callback_config(&cbus_wbo, wbo_module_cb);

    // Setup canbus (rusEFI WBO module)
    can2040_setup(&cbus_external, 1);
    can2040_callback_config(&cbus_external, cbus_ext_cb);

    // Enable irqs (wbo)
    irq_set_exclusive_handler(PIO0_IRQ_0_IRQn, PIOx_IRQHandler);
    NVIC_SetPriority(PIO0_IRQ_0_IRQn, 1);
    NVIC_EnableIRQ(PIO0_IRQ_0_IRQn);

    // Enable irqs (external)
    irq_set_exclusive_handler(PIO1_IRQ_0_IRQn, PIOx_IRQHandler2);
    NVIC_SetPriority(PIO1_IRQ_0_IRQn, 1);
    NVIC_EnableIRQ(PIO1_IRQ_0_IRQn);

    // Start canbus
    can2040_start(&cbus_wbo, sys_clock, bitrate, 4, 5);
    can2040_start(&cbus_external, sys_clock, bitrate, 6, 7);
}

void setup()
{
    canbus_setup();
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

u32_t previousMillis = 0;
u16_t interval = 500;

// the loop routine runs over and over again forever:
void loop()
{
    u32_t currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        wbo_module_loop();
        Serial.print("WBO Value:");
        Serial.println(currentO2);
    }
    updateDisplayValue();
}

void sendRusEfiWbo(struct can2040_msg *msg)
{
    can2040_transmit(&cbus_external, msg);
}