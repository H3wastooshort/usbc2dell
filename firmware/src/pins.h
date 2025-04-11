#pragma once
#include <Arduino.h>

const uint8_t PIN_ID = PIN_PB4;

const uint8_t PIN_FUSB_INT = PIN_PB2;

const uint8_t PIN_PWR_EN = PIN_PB3;

//LED
const uint8_t PIN_LED_R = PIN_PA5;
const uint8_t PIN_LED_G = PIN_PA6;
const uint8_t PIN_LED_B = PIN_PA7;

//DIP Switchesa
const uint8_t PIN_SW_EMU_TYPE_0 = PIN_PC0;
const uint8_t PIN_SW_EMU_TYPE_1 = PIN_PC1;
const uint8_t PIN_SW_USE_PPS = PIN_PC2;
const uint8_t PIN_SW_LED_BRIGHT = PIN_PC3;



//automatic, ignore
constexpr uint8_t PINS_LED[3] = {PIN_LED_R,PIN_LED_G,PIN_LED_B};

