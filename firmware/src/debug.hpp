#pragma once
#include <Arduino.h>
#include "emu_controller.hpp"
#include "chg_emu_dell.hpp"

enum logger_msgtype_e {
    MSGT_INIT=0,
    MSGT_NEGOTIATION_STATE,
    MSGT_DELL_CHG_ID,
};

class Logger final {
    Logger() = delete;

    public:
    static void setup();
    static void log(const negotiantion_states_e state);
    static void log(const dell_chg_id_t& dell);
};
