#pragma once
#include <PD_UFP.h>
#include "chg_emu.h"

enum negotiantion_states_e
{
    NEG_INIT = 0,
    NEG_WAIT_FOR_PD_TRANS,
    NEG_WAIT_FOR_PD,
    NEG_GOT_20V_TRANS,
    NEG_GOT_20V,
    NEG_GOT_PPS_TRANS,
    NEG_GOT_PPS,
};

class EmuController {
    //not copyable
    EmuController(const EmuController&) = delete;
    EmuController& operator=(const EmuController&) = delete;
    
    const bool use_pps;
    
    chg_emu_c *emulator;
    PD_UFP_c pd;
    TwoWire *i2c;

    negotiantion_states_e negotiantion_state;

    void run_sm();
    
    public:
    EmuController(TwoWire* i2c, const bool use_pps, chg_emu_c *emulator);
    void setup();
    void loop();
};

