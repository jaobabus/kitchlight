#include "lightapi.h"



void Light::init()
{
    pinit();
    load(_settings);
}

Light::Settings Light::defaultSettings()
{
    return {
        1ul * 60 * 60 * 1000, // 1h
        6ul * 60 * 60 * 1000, // 6h
        1500, // 1.5s
        {
            Light::Led::defaultSettings(),
            Light::Led::defaultSettings(),
        }
    };
}

void Light::event(TickEvent ev)
{
    _time_us += ev.elapsed_us;
    ledsEvent(ev);
}

void Light::event(ButtonEvent ev)
{
    ledsEvent(ev);
}


void Light::ledsEvent(TickEvent tick)
{
    _leds[0].tick(tick.elapsed_us);
    setLed(0, _leds[0].value());
    _leds[1].tick(tick.elapsed_us);
    setLed(1, _leds[1].value());
    _click.tick(tick.elapsed_us);
    _timeout.tick(tick.elapsed_us);

    if (not _timeout.running() && _state != State::Disabled) {
        _leds[0].setEnabled(false);
        _leds[1].setEnabled(false);
        _state = State::Disabled;
    }
}

void Light::ledsEvent(ButtonEvent ev)
{
    if (ev.state) {
        switch (_state)
        {
        case State::Disabled:
            _timeout.start(_settings.timeout_ms * 1000);
            _click.start(_settings.double_click_ms * 1000);
            _leds[0].setEnabled(true);
            _leds[1].setEnabled(true);
            _state = State::Enabled;
            break;

        case State::Enabled:
            if (_click.running()) {
                _timeout.start(_settings.long_timeout_ms * 1000);
                _state = State::LongTimeoutEnabled;
            }
            else {
                _leds[0].setEnabled(false);
                _leds[1].setEnabled(false);
                _state = State::Disabled;
            }
            break;

        case State::LongTimeoutEnabled:
            _leds[0].setEnabled(false);
            _leds[1].setEnabled(false);
            _state = State::Disabled;
            break;

        default:
            _state = State::Disabled;
        }
    }
}
