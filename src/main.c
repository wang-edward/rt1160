#include "fsl_device_registers.h"  // pulls in MIMXRT1166_cm7.h, periph headers, core_cm7
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

#define LED_GPIO        GPIO1
#define LED_PIN         6U

static void delay(volatile uint32_t d) {
    while (d--) { __asm__("nop"); }
}

int main(void) {
    // 1) Enable clocks for IOMUXC & GPIO1
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    // 2) Pin-mux GPIO_AD_B0_06 to GPIO1_IO06 (ALT5), no input DAISY, no extra config
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_06_GPIO1_IO06,  // mux register
        5U,                               // ALT5 = GPIO1_IO06
        0U,                               // no input select
        0U,                               // no daisy
        0U                                // no config here
    );
    // 3) Configure pad: pull-up, 40Ω drive strength, hysteresis off
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_06_GPIO1_IO06,
        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK           | // enable pull/keeper
        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK           | // pull-up
        IOMUXC_SW_PAD_CTL_PAD_PUS(2)             | // 100 KΩ pull-up
        IOMUXC_SW_PAD_CTL_PAD_DSE(6)             | // 40Ω drive
        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK             // hysteresis (optional)
    );

    // 4) Init GPIO as digital output, default low
    gpio_pin_config_t gpio_cfg = {
        .direction  = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    GPIO_PinInit(LED_GPIO, LED_PIN, &gpio_cfg);

    // 5) Blink
    for (;;) {
        GPIO_TogglePinsOutput(LED_GPIO, 1U << LED_PIN);
        delay(2000000);
    }

    return 0;
}
