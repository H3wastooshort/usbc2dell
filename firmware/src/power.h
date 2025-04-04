#include "pins.h"
void set_power(bool state) {
    digitalWrite(PIN_PWR_EN, state);
}