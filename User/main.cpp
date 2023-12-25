/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/


#include <Debug/debug.h>
#include <Core/millis.h>
#include <ch32v00x.h>

#include "peripheral.h"
#include "lightapi.h"



constexpr uint32_t update_period_us = 8333;


class ImplLight : public Light
{
public:
    ImplLight() : Light() {}

public:
    virtual void pinit() {}

    virtual void setLed(int id, uint8_t pwm) {
        if (id == 0) setLed1(pwm);
        if (id == 1) setLed2(pwm);
    }

    virtual void load(Settings& settings) {
        settings = Light::defaultSettings();
    }

    virtual void save(Settings const& settings) {}

};


bool checkButton1Changed(bool& value)
{
    static bool last_value = false;
    static uint64_t last_ms = 0;
    bool current = checkButton1();
    if ((millis() - last_ms) > 100 && current != last_value) {
        last_value = current;
        last_ms = millis();
        value = last_value;
        return true;
    }
    return false;
}


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    millis_init();

    peripheralInit();

    while (false) {
        setLed1(255);
        setLed2(255);
        delay(250);
        setLed1(0);
        setLed2(0);
        delay(250);
    }

    ImplLight light;
    light.init();
    uint64_t last_us = 0;

    while (true) {
        volatile auto elapsed = micros() - last_us;
        {
            bool value;
            if (checkButton1Changed(value))
                light.event(Light::ButtonEvent{0, value});
        }
        {
            light.event(Light::TickEvent{(uint32_t)elapsed});
        }
        last_us = micros();
        delay_us(update_period_us);
    }
}
