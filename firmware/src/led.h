#include <Arduino.h>
#include <pins.h>

//colorblind-friendly palette
constexpr uint8_t LED_COLOR_NONE[3] = {0xfe, 0x61, 0x00};
constexpr uint8_t LED_COLOR_20V[3] = {0x78, 0x5e, 0xf0};
constexpr uint8_t LED_COLOR_PPS[3] = {0x64, 0x8f, 0xff};

void set_led_color(const uint8_t color[3]) {
    for (uint8_t i=0; i<3; i++)analogWrite(PINS_LED[i],color[i]);
}
