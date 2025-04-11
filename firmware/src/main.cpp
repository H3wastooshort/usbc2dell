#include <Arduino.h>
#include <Wire.h>
#include "pins.h"
#include "globals.h"
#include "chg_emu_dell.h"
#include "led.h"
#include "state_machine.h"

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
    // TODO: read config DIP
    bool emu_mode = (digitalRead(PIN_SW_EMU_TYPE_0) << 1) | (digitalRead(PIN_SW_EMU_TYPE_0) << 0);
    bool use_pps = digitalRead(PIN_SW_USE_PPS);
    // TODO: init appropriate emu
    switch (emu_mode&0b11) {
        default: {
            selected_emu = dynamic_cast<chg_emu_c*>(new chg_emu_dell_c(PIN_ID)); //TODO: why does this not work?
        }
    }
    selected_emu->setup();
    // TODO: start pd
    Wire.swap();
    Wire.begin();
    if (use_pps)
        PD_UFP.init_PPS(PIN_FUSB_INT, PPS_V(19.5), PPS_A(5), PD_POWER_OPTION_MAX_20V);
    else
        PD_UFP.init(PIN_FUSB_INT, PD_POWER_OPTION_MAX_20V);
}

void loop()
{
    PD_UFP.run();
    run_sm();
    selected_emu->loop();
}