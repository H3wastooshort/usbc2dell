#include "led.hpp"
#include <Arduino.h>

void set_led_color(const uint8_t color[3]) {
    uint8_t dim = (!digitalRead(PIN_SW_LED_BRIGHT)) ? 4 : 8; //divide by 4, if dimming enabled
    //for (uint8_t i=0; i<3; i++)analogWrite(PINS_LED[i], color[i] / dim);
    for (uint8_t i=0; i<3; i++) digitalWrite(PINS_LED[i], color[i] > 127);
}
