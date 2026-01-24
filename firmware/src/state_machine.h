#pragma once
#include <PD_UFP.h>
#include "chg_emu.h"
#include "globals.h"
#include "power.h"
#include "led.h"

enum negotiantion_state_machine_e
{
    NEG_INIT = 0,
    NEG_WAIT_FOR_PD_TRANS,
    NEG_WAIT_FOR_PD,
    NEG_GOT_20V_TRANS,
    NEG_GOT_20V,
    NEG_GOT_PPS_TRANS,
    NEG_GOT_PPS,
} negotiantion_state;

// TODO: make class
void run_sm() {
    switch (negotiantion_state) {
        default:
        case NEG_INIT:
            negotiantion_state=NEG_WAIT_FOR_PD_TRANS;
            break;

        case NEG_WAIT_FOR_PD_TRANS:
            selected_emu->disable();
            set_power(false);
            set_led_color(LED_COLOR_NONE);
            negotiantion_state=NEG_WAIT_FOR_PD;
            break;

        case NEG_WAIT_FOR_PD:
            if (PD_UFP.is_PPS_ready())
                negotiantion_state = NEG_GOT_PPS_TRANS;
            else if (PD_UFP.is_power_ready())
                negotiantion_state = NEG_GOT_20V_TRANS;
            break;

        case NEG_GOT_20V_TRANS:
            set_led_color(LED_COLOR_20V);
            selected_emu->set_param(PD_UFP.get_voltage() * 50 /*mV*/, PD_UFP.get_current() * 10 /*mA*/);
            selected_emu->enable();
            set_power(true);
            negotiantion_state = NEG_GOT_20V;
            break;
        case NEG_GOT_20V: if (!PD_UFP.is_power_ready()) negotiantion_state=NEG_WAIT_FOR_PD_TRANS;

        case NEG_GOT_PPS_TRANS:
            set_led_color(LED_COLOR_PPS);
            selected_emu->set_param(PD_UFP.get_voltage() * 20 /*mV*/, PD_UFP.get_current() * 50 /*mA*/);
            selected_emu->enable();
            set_power(true);
            negotiantion_state = NEG_GOT_PPS;
            break;
        case NEG_GOT_PPS: if (!PD_UFP.is_PPS_ready()) negotiantion_state=NEG_WAIT_FOR_PD_TRANS;
    }
}
