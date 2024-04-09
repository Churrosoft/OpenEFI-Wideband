#include <Arduino.h>
#include "pico/stdlib.h"

extern "C" {
  #include "can2040.h"
}

static struct can2040 cbus_wbo;
static struct can2040 cbus_external;

static void cbus_wbo_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Add message processing code here...
}
static void cbus_ext_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Add message processing code here...
}


static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus_wbo);
}

static void PIOx_IRQHandler2(void)
{
    can2040_pio_irq_handler(&cbus_external);
}

void canbus_setup(void)
{
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus (rusEFI WBO module)
    can2040_setup(&cbus_wbo, 0);
    can2040_callback_config(&cbus_wbo, cbus_wbo_cb);

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

// the setup routine runs once when you press reset:
void setup()
{
    // initialize the digital pin as an output.
    canbus_setup();
    pinMode(LED_BUILTIN, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);                     // wait for a second
}