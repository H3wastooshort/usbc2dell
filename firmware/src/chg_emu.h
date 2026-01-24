#pragma once
#include <Arduino.h>
class chg_emu_c {
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void set_param(uint32_t millivolt, uint32_t milliamp) = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
};