#include <stdint.h>
#include "fsl_gpio.h"

// 1) Clock gate register for GPIO1 (CCM CCGR5)
#define CCM_CCGR5       (*(volatile uint32_t*)0x403F816C)
#define CCM_CCGR5_GPIO1 (3U << 2)      // bits [3:2] enable clocks for GPIO1

// 2) IOMUXC pad mux & pad control for GPIO_AD_B0_06 (LED1)
//    see RM: SW_MUX_CTL_PAD offset = 0x8214, SW_PAD_CTL_PAD = 0x8414
#define IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_06 \
                        (*(volatile uint32_t*)0x401F8214)
#define IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_06 \
                        (*(volatile uint32_t*)0x401F8414)
#define MUX_ALT5_GPIO    5U
#define PAD_CTL_CFG      (0x10B0U)     // PKE on, PUE pull-up, PUS=2 (100KΩ), DSE=6 (40Ω), HYS off

// 3) GPIO1 registers: direction & data
#define GPIO1_BASE      0x401B8000u
#define GPIO1_GDIR      (*(volatile uint32_t*)(GPIO1_BASE + 0x04))
#define GPIO1_DR        (*(volatile uint32_t*)(GPIO1_BASE + 0x10))
#define LED_PIN         6U

static void delay(volatile uint32_t d) {
    while (d--) { __asm__("nop"); }
}

int main(void) {
    // Enable clock to GPIO1
    CCM_CCGR5 |= CCM_CCGR5_GPIO1;

    // Mux pad to ALT5 (GPIO1_IO06)
    IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_06 = MUX_ALT5_GPIO;
    // Set pad config (pull-up, drive strength, etc.)
    IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_06 = PAD_CTL_CFG;

    // Configure LED pin as output
    GPIO1_GDIR |= (1U << LED_PIN);

    // Blink forever
    for (;;) {
        GPIO1_DR ^= (1U << LED_PIN);
        delay(2000000);
    }
    return 0;
}
