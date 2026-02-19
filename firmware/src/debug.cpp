#include "debug.hpp"
#include <Arduino.h>

const char ASCII_SOH = 1;
const char ASCII_SOT = 2;
const char ASCII_EOT = 3;

void Logger::setup() {
    Serial0.swap(1);
    Serial0.begin(115200);
    Serial0.write(ASCII_SOH);
    Serial0.write(MSGT_INIT);
}

void Logger::log(const negotiantion_states_e state) {
    Serial0.write(ASCII_SOH);
    Serial0.write(MSGT_NEGOTIATION_STATE);
    Serial0.write(ASCII_SOT);
    Serial0.write(static_cast<uint8_t>(state));
    Serial0.write(ASCII_EOT);
}

void Logger::log(const dell_chg_id_t& dell) {
    Serial0.write(ASCII_SOH);
    Serial0.write(MSGT_DELL_CHG_ID);
    Serial0.write(ASCII_SOT);
    Serial0.write(reinterpret_cast<const char*>(&dell),sizeof(dell_chg_id_t));
    Serial0.write(ASCII_EOT);
}

