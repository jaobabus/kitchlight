#pragma once
#ifndef USER_PERIPHERAL_H_
#define USER_PERIPHERAL_H_

#include <inttypes.h>
#include <ch32v00x.h>


struct GPIOPin
{
    GPIO_TypeDef* gpio;
    uint16_t pin;
};


/** GPIO Scheme
 *   - PD1(8) - SWDIO
 *   - PC4(7) - LED2 Out (T1CH4)
 *   - PA1(1) - Tx / WS2812 Out
 *   - PC1(5) - Rx
 *   - PA2(3) - Button In
 *   - PC2(6) - LED1 Out (T2CH2)
 */

constexpr GPIOPin led1     = { GPIOC, GPIO_Pin_4 };
constexpr GPIOPin led2     = { GPIOC, GPIO_Pin_2 };
constexpr GPIOPin ws2812_1 = { GPIOA, GPIO_Pin_1 };
constexpr GPIOPin button   = { GPIOA, GPIO_Pin_2 };



void peripheralInit();

void setLed1(uint16_t pwm);
void setLed2(uint16_t pwm);
void setWS2812Led1(uint32_t rgb);
bool checkButton1();


#endif /* USER_PERIPHERAL_H_ */
