#include <Arduino.h>
class chg_emu_c {
public:
    virtual void setup() {}
    virtual void loop() {}
    virtual void set_param(uint32_t millivolt, uint32_t milliamp) {}
    virtual void enable() {}
    virtual void disable() {}
};