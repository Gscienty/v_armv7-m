#include "machine.h"

void it_advance(machine_t& machine) {
    machine.it_state &= mask_bits<ubit4_t, ubit8_t, 0, 3>(machine.it_state) == 0 ? (0) : 
        (binary<11100000>::val | SL<ubit8_t, 5>(mask_bits<ubit8_t, ubit8_t, 0, 5>(machine.it_state), 1));
}

bool in_it_block(machine_t& machine) { return mask_bits<ubit8_t, ubit4_t, 0, 4>(machine.it_state) != 0; }
bool last_in_it_block(machine_t& machine) { return mask_bits<ubit8_t, ubit4_t, 0, 4>(machine.it_state) == binary<1000>::val; }

void alu_write_pc(ubit32_t value, machine_t& machine) { machine.arm.r15 = value; }

couple<sr_type, ubit8_t> decode_imm_shift(ubit2_t type, ubit8_t imm5) {
    imm5 = mask_bits<ubit8_t, ubit8_t, 0, 5>(imm5);
    switch(type) {
        case binary<00>::val : return { lsl, imm5 };
        case binary<01>::val : return { lsr, imm5 == 0 ? 32 : imm5 };
        case binary<10>::val : return { asr, imm5 == 0 ? 32 : imm5 };
        case binary<11>::val : if (imm5 == 0) return { rrx, 1 }; else return { ror, imm5 };
    }
}

sr_type decode_reg_shift(ubit2_t type) {
    switch(type) {
        case binary<00>::val : return lsl;
        case binary<01>::val : return lsr;
        case binary<10>::val : return asr;
        case binary<11>::val : return ror;
    }
}
