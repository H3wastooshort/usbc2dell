#pragma once
#include <Arduino.h>
#include "pins.h"

//colorblind-friendly palette
constexpr uint8_t LED_COLOR_BOOT[3] = {0xff, 0xff, 0xff};
constexpr uint8_t LED_COLOR_ERR[3] = {0xdc, 0x26, 0x7f};
constexpr uint8_t LED_COLOR_NONE[3] = {0xfe, 0x61, 0x00};
constexpr uint8_t LED_COLOR_20V[3] = {0x78, 0x5e, 0xf0};
constexpr uint8_t LED_COLOR_PPS[3] = {0x64, 0x8f, 0xff};

void set_led_color(const uint8_t color[3]) {
    uint8_t dim = (!digitalRead(PIN_SW_LED_BRIGHT)) ? 4 : 8; //divide by 4, if dimming enabled
    for (uint8_t i=0; i<3; i++)analogWrite(PINS_LED[i], color[i] / dim);
}
