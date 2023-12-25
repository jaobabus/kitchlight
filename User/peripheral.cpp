#include "peripheral.h"

#include <ch32v00x.h>


void InitGPIOs();
void TIMs_PWMOut_Init(uint16_t arr, uint16_t psc);


void peripheralInit()
{
    InitGPIOs();
    TIMs_PWMOut_Init(255, 125); // 1.5KHz
}

void setLed1(uint16_t pwm)
{
    TIM1->CH4CVR = pwm;
}

void setLed2(uint16_t pwm)
{
    TIM2->CH2CVR = pwm;
}

void setWS2812Led1(uint32_t rgb)
{

}

bool checkButton1()
{
    return GPIO_ReadInputDataBit(button.gpio, button.pin);
}



void initGPIO(GPIOPin gpio, GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef  gpiois = {0};
    gpiois.GPIO_Pin = gpio.pin;
    gpiois.GPIO_Speed = GPIO_Speed_50MHz;
    gpiois.GPIO_Mode = mode;
    GPIO_Init(gpio.gpio, &gpiois);
}

void InitGPIOs()
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE );

    initGPIO(led1, GPIO_Mode_AF_PP);
    AFIO->PCFR1 = AFIO_PCFR1_TIM2_REMAP_PARTIALREMAP1;
    AFIO->EXTICR = 0;
    initGPIO(led2, GPIO_Mode_AF_PP);
    initGPIO(ws2812_1, GPIO_Mode_Out_PP);

    initGPIO(button, GPIO_Mode_IN_FLOATING);
}

void TIMs_PWMOut_Init(uint16_t arr, uint16_t psc)
{
    {
        TIM_OCInitTypeDef TIM_OCInitStructure = { 0 };
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = { 0 };

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

        TIM_TimeBaseInitStructure.TIM_Period = arr;
        TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
        TIM_OC4Init(TIM1, &TIM_OCInitStructure);

        TIM_CtrlPWMOutputs(TIM1, ENABLE);
        TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
        TIM_ARRPreloadConfig(TIM1, ENABLE);
        TIM_Cmd(TIM1, ENABLE);
    }

    {
        TIM_OCInitTypeDef TIM_OCInitStructure = { 0 };
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = { 0 };

        RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

        TIM_TimeBaseInitStructure.TIM_Period = arr;
        TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
        TIM_OC2Init(TIM2, &TIM_OCInitStructure);

        TIM_CtrlPWMOutputs(TIM2, ENABLE);
        TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
        TIM_ARRPreloadConfig(TIM2, ENABLE);
        TIM_Cmd(TIM2, ENABLE);
    }
}
