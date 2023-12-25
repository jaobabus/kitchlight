#pragma once

#ifndef __MILLIS_H
#define __MILLIS_H

#include <inttypes.h>

#include <ch32v00x.h>


void millis_init(GPIO_TypeDef* gpio = 0, uint16_t led_pin = {0}, int millis_us_period = 1000000);

uint64_t millis();
uint64_t micros();

void delay(uint32_t ms);
void delay_us(uint32_t us);



#endif // __MILLIS_H
