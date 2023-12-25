#pragma once

#include <stdio.h>
#include <stdint.h>

#include "utils.h"



class Timer
{
public:
    Timer(uint16_t max_value = 0) : _time(0), _max_time(0), _max_value(max_value) {}

    void start(uint32_t us) {
        _time = 0;
        _max_time = us;
    }

    void tick(uint32_t elapsed_us) {
        if (running())
            _time += elapsed_us;
    }

    uint16_t value() const {
        return _time * _max_value / _max_time;
    }

    uint32_t time() const {
        return _time;
    }

    bool running() const {
        return _time < _max_time;
    }

private:
    uint32_t _time;
    uint32_t _max_time;
    const uint16_t _max_value;

};


class Light
{
public:
    class Led
    {
    public:
        struct Settings {
            uint16_t rizing_ms;
        };

    public:
        Led(Settings& settings) : _enabled(false), _timer(180), _settings(settings) {}

        void tick(uint32_t elapsed_us) {
            _timer.tick(elapsed_us);
        }

        uint16_t value() const {
            uint16_t value = (_timer.running() ? (sin16(_timer.value() - 90) + 32768) / 256 : 255);
            if (_enabled)
                return value;
            else
                return 255 - value;
        }

        void setEnabled(bool state) {
            _enabled = state;
            _timer.start((uint32_t)_settings.rizing_ms * 1000u);
        }

        static Settings defaultSettings() {
            return { 2000 };
        }

    private:
        bool _enabled;
        Timer _timer;
        Settings& _settings;

    };

public:
    struct ButtonEvent
    {
        int number;
        bool state;
    };

    struct TickEvent
    {
        uint32_t elapsed_us;
    };

    enum class State
    {
        Disabled,
        Enabled,
        LongTimeoutEnabled,
    };

    struct Settings
    {
        uint32_t timeout_ms;
        uint32_t long_timeout_ms;
        uint16_t double_click_ms;
        Led::Settings leds[2];
    };


public:
    Light()
        : _time_us(0),
          _state(State::Disabled),
          _leds{ { _settings.leds[0] }, { _settings.leds[1] } }
    {}
    virtual ~Light() {}

    void init();
    static Settings defaultSettings();

    void event(TickEvent tick);
    void event(ButtonEvent ev);


private:
    void ledsEvent(TickEvent tick);
    void ledsEvent(ButtonEvent ev);


protected:
    virtual void pinit() {}
    virtual void setLed(int id, uint8_t pwm) = 0; // 0-255

    virtual void load(Settings& settings) = 0;
    virtual void save(Settings const& settings) = 0;

private:
    uint32_t _time_us;
    Settings _settings;

    State _state;
    Led _leds[2];
    Timer _click;
    Timer _timeout;

};


