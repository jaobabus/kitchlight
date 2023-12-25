#include "millis.h"

#include <ch32v00x.h>

#include "debug.h"



extern uint32_t SystemCoreClock;


static uint32_t millis_period;
static uint64_t millis_up_counter;
static uint16_t dbg_pin;
static GPIO_TypeDef* dbg_gpio;

void millis_init(GPIO_TypeDef* led_gpio, uint16_t led_pin, int us_period) {
    millis_period = (uint64_t)us_period * SystemCoreClock / 1000000;
    millis_up_counter = 0;

    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR &= ~(1 << 0);
    SysTick->CNT = 0;
    SysTick->CMP = 0xFFFFFFFF;
    SysTick->CTLR = 0x0F;

    dbg_pin = led_pin;
    dbg_gpio = led_gpio;
    if (dbg_gpio) {
        GPIO_InitTypeDef  gpiois = {0};
        gpiois.GPIO_Pin = dbg_pin;
        gpiois.GPIO_Speed = GPIO_Speed_50MHz;
        gpiois.GPIO_Mode = GPIO_Mode_Out_OD;
        GPIO_Init(dbg_gpio, &gpiois);
        GPIO_WriteBit(dbg_gpio, dbg_pin, Bit_SET);
    }
}

uint64_t millis() {
    return micros() / 1000;
}

uint64_t micros() {
    const auto fact = SystemCoreClock / 1000000;
    auto val = SysTick->CNT / fact;
    return val;
}

void delay(uint32_t ms) {
    uint32_t start = millis();
    while (millis() - start < ms); //dbgprintf("millis=%lu\r\n", millis());
}

void delay_us(uint32_t us) {
    uint32_t start = micros();
    while (micros() - start < us);
}


extern "C" void SysTick_Handler(void) WCH_ISR_FAST;
extern "C" void SysTick_Handler(void)
{
    if (dbg_gpio) {
        GPIO_WriteBit(dbg_gpio, dbg_pin, Bit_RESET);
        for (volatile int i = 0; i < 50000; i++)
            ;
        GPIO_WriteBit(dbg_gpio, dbg_pin, Bit_SET);
    }
    millis_up_counter += millis_period;
    SysTick->SR = 0;
}

