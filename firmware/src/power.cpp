#include "power.hpp"
#include "pins.hpp"

void set_power(bool state) {
    if (state) pinMode(PIN_PWR_EN, INPUT_PULLUP); //use built-in resistor as current limiting
    else {
        digitalWrite(PIN_PWR_EN, LOW);
        pinMode(PIN_PWR_EN, OUTPUT);
    }
}