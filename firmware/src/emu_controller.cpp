#include "emu_controller.hpp"
#include <PD_UFP.h>
#include "chg_emu.hpp"
#include "power.hpp"
#include "led.hpp"
#include "debug.hpp"

EmuController::EmuController(TwoWire* i2c, const bool use_pps, chg_emu_c *emulator) : i2c(i2c), use_pps(use_pps), emulator(emulator), negotiantion_state(NEG_INIT) {};

void EmuController::sm_change_state(const negotiantion_states_e new_state) {
    this->negotiantion_state=new_state;
    Logger::log(new_state);
}

void EmuController::sm_run() {
    switch (this->negotiantion_state) {
        default:
        case NEG_INIT:
            sm_change_state(NEG_WAIT_FOR_PD_TRANS);
            break;

        case NEG_WAIT_FOR_PD_TRANS:
            this->emulator->disable();
            set_power(false);
            set_led_color(LED_COLOR_NONE);
            sm_change_state(NEG_WAIT_FOR_PD);
            break;

        case NEG_WAIT_FOR_PD:
            if (pd.is_PPS_ready())
                sm_change_state(NEG_GOT_PPS_TRANS);
            else if (pd.is_power_ready())
                sm_change_state(NEG_GOT_20V_TRANS);
            break;

        case NEG_GOT_20V_TRANS:
            set_led_color(LED_COLOR_20V);
            this->emulator->set_param(pd.get_voltage() * 50 /*mV*/, pd.get_current() * 10 /*mA*/);
            this->emulator->enable();
            set_power(true);
            sm_change_state(NEG_GOT_20V);
            break;
        case NEG_GOT_20V: if (!pd.is_power_ready()) sm_change_state(NEG_WAIT_FOR_PD_TRANS);

        case NEG_GOT_PPS_TRANS:
            set_led_color(LED_COLOR_PPS);
            this->emulator->set_param(pd.get_voltage() * 20 /*mV*/, pd.get_current() * 50 /*mA*/);
            this->emulator->enable();
            set_power(true);
            sm_change_state(NEG_GOT_PPS);
            break;
        case NEG_GOT_PPS: if (!pd.is_PPS_ready()) sm_change_state(NEG_WAIT_FOR_PD_TRANS);
    }
}

void EmuController::setup() {
    this->emulator->setup();
    //this->i2c->swap(0);
    this->i2c->usePullups();
    this->i2c->setClock(10000);
    this->i2c->begin();
    this->i2c->usePullups();

    this->i2c->beginTransmission (0x22);
    if (this->i2c->endTransmission () != 0) while (1) { //blink if missing fusb
        set_led_color(LED_COLOR_BOOT);
        delay(500);
        set_led_color(LED_COLOR_ERR);
        delay(500);
    }

    if (use_pps)
        pd.init_PPS(PIN_FUSB_INT, PPS_V(19.5), PPS_A(5), PD_POWER_OPTION_MAX_20V);
    else
        pd.init(PIN_FUSB_INT, PD_POWER_OPTION_MAX_20V);
}

void EmuController::loop() {
    pd.run();
    this->sm_run();
    this->emulator->loop();
}