enum negotiantion_state_machine_e {
    NEG_INIT=0,
    NEG_WAIT_FOR_PD_TRANS,
    NEG_WAIT_FOR_PD,
    NEG_GOT_20V_TRANS,
    NEG_GOT_20V,
    NEG_GOT_PPS_TRANS,
    NEG_GOT_PPS,
} negotiantion_state;