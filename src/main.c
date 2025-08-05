// src/c/main.c

#include "fsl_clock.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

#define LED_GPIO    GPIO1
#define LED_PIN     6U      // On EVK this is the user LED1

static void delay(volatile uint32_t n) {
    while (n--) { __asm__("nop"); }
}

int main(void) {
    // Enable clocks
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    // Pin-mux LED pin
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_06_GPIO1_IO06, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_06_GPIO1_IO06, 0x10B0U);

    // Init as output, default low
    gpio_pin_config_t cfg = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0,
        .interruptMode = kGPIO_NoIntmode
    };
    GPIO_PinInit(LED_GPIO, LED_PIN, &cfg);

    // Blink forever
    for (;;) {
        GPIO_TogglePinsOutput(LED_GPIO, 1U << LED_PIN);
        delay(2000000);
    }
}

