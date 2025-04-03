#include <Arduino.h>
class chg_emu_c {
    virtual void loop() {}
    virtual void set_param(uint32_t millivolt, uint32_t milliamp) {}
    virtual void enable() {}
    virtual void disable() {}
};