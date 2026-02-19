#include <Arduino.h>
#include <Wire.h>
#include "pins.h"
#include "chg_emu_dell.h"
#include "led.h"
#include "power.h"
#include "emu_controller.h"

EmuController* ec = NULL;

void setup()
{
    set_power(false);
    // TODO: init pins
    pinMode(PIN_SW_EMU_TYPE_0, INPUT_PULLUP);
    pinMode(PIN_SW_EMU_TYPE_1, INPUT_PULLUP);
    pinMode(PIN_SW_USE_PPS, INPUT_PULLUP);
    pinMode(PIN_SW_LED_BRIGHT, INPUT_PULLUP);
    //init LED
    for (uint8_t i = 0; i<3; i++) {digitalWrite(PINS_LED[i], LOW); pinMode(PINS_LED[i], OUTPUT);}
    set_led_color(LED_COLOR_BOOT);

    uint8_t emu_mode = ( (digitalRead(PIN_SW_EMU_TYPE_0) << 1) | (digitalRead(PIN_SW_EMU_TYPE_0) << 0) ) & 0b11;
    bool use_pps = digitalRead(PIN_SW_USE_PPS);
    
    
    //init appropriate emu
    chg_emu_c* selected_emu = NULL;
    switch (emu_mode) {
        //TODO: HP emulator
        case 0b11:
        default:
            {
                selected_emu = dynamic_cast<chg_emu_c*>(
                    new chg_emu_dell_c(PIN_ID)
                );
            } break;
    }

    //Init Emu
    ec = new EmuController(&Wire,use_pps,selected_emu);
    ec->setup();
}

void loop()
{
    ec->loop();
}