#ifndef _MACHINE_
#define _MACHINE_

#include "cpu.h"

struct machine_t {
    arm_t arm;

    ubit8_t it_state;
};

void it_advance(machine_t& machine);
bool in_it_block(machine_t& machine);
bool last_in_it_block(machine_t& machine);

void alu_write_pc(ubit32_t value, machine_t& machine);

couple<sr_type, ubit8_t> decode_imm_shift(ubit2_t type, ubit8_t imm5);
sr_type decode_reg_shift(ubit2_t type);

template<typename T, ubit8_t n> couple<T, ubit1_t> shift_c(T value, sr_type type, ubit8_t amount, ubit1_t carry_in) { 
    ASSERT(type == rrx && amount != 1)

    if(amount == 0) return { value, carry_in };
    else switch (type) {
        case lsl : return SL_C <T, n>(value, amount  );
        case lsr : return SR_C <T, n>(value, amount  );
        case asr : return SR_C <T, n>(value, amount  );
        case ror : return ROR_C<T, n>(value, amount  );
        case rrx : return RRX_C<T, n>(value, carry_in);
    }
}

template<typename T, ubit8_t n> T shift(T value, sr_type type, ubit8_t amount, ubit1_t carry_in) { return shift_c<T, n>(value, type, amount, carry_in).item1; }

#endif