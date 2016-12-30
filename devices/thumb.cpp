#include "thumb.h"

couple<ubit32_t, ubit1_t> thumb_expand_imm_c (ubit12_t imm12, ubit1_t carry_in) {
    if(mask_bits<ubit2_t, ubit12_t, 10, 2>(imm12) == binary<00>::val) {
        ubit32_t m = mask_bits<ubit32_t, ubit12_t, 0, 8>(imm12);
        switch(mask_bits<ubit2_t, ubit12_t, 8, 2>(imm12)) {
            case binary<00>::val : return { m, carry_in };
            case binary<01>::val : if (m == 0) { /*unpredicttable*/ } 
                return { (m << 16) | (m)       , carry_in };
            case binary<10>::val : if (m == 0) { /*unpredicttable*/ }
                return { (m << 24) | (m << 8)  , carry_in };
            case binary<11>::val : if (m == 0) { /*unpredicttable*/ }
                return { (m << 24) | (m << 16) | (m << 8) | (m) , carry_in };
        }
    }
    else return ROR_C<ubit32_t, 32>(binary<10000000>::val | mask_bits<ubit32_t, ubit12_t, 0, 7>(imm12), mask_bits<ubit8_t, ubit12_t, 7, 5>(imm12));
}

ubit32_t thumb_expand_imm(ubit12_t imm12, bool apsr_cf) { return thumb_expand_imm_c(imm12, apsr_cf ? 1 : 0).item1; }

void adc_imm(ubit16_t opcode_h , ubit16_t opcode_l, machine_t& machine) {
    ubit4_t rdx = mask_bits<ubit4_t, ubit16_t, 8, 4>(opcode_l);
    ubit4_t rnx = mask_bits<ubit4_t, ubit16_t, 0, 4>(opcode_h);
    if(BAD_R(rdx, machine.arm) || BAD_R(rdx, machine.arm)) { /*unpredicttable*/ return ; }
    
    triad<ubit32_t, ubit1_t, ubit1_t> result = add_with_carry<ubit32_t, 32>(
        GET_R(rdx, machine.arm), 
        thumb_expand_imm(
                        mask_bits<ubit12_t, ubit16_t, 10, 1>(opcode_h) << 11 |
                        mask_bits<ubit12_t, ubit16_t, 12, 3>(opcode_l) << 8  |
                        mask_bits<ubit12_t, ubit16_t, 0 , 8>(opcode_l),
                        false
                    ), 
        G_CF(machine.arm) ? 1 : 0
    );

    GET_R(rnx, machine.arm) = result.item1;

    if(mask_bit<ubit1_t, ubit16_t, 4>(opcode_h) == 1) {
        mask_bit<ubit1_t, ubit32_t, 31>(result.item1) == 1 ? S_NF(machine.arm) : C_NF(machine.arm);
        result.item1 == 0 ? S_ZF(machine.arm) : C_ZF(machine.arm);
        result.item2 == 1 ? S_CF(machine.arm) : C_CF(machine.arm);
        result.item3 == 1 ? S_VF(machine.arm) : C_VF(machine.arm);
    }
}

void adc_reg_t1(ubit16_t opcode, machine_t& machine) {
    ubit4_t rdnx = mask_bits<ubit4_t, ubit16_t, 0, 3>(opcode);
    ubit4_t rmx  = mask_bits<ubit4_t, ubit16_t, 3, 3>(opcode);

    ubit32_t shifted = shift<ubit32_t, 32>(GET_R(rmx, machine.arm), sr_type::lsl, 0, G_CF(machine.arm) ? 1 : 0);
    triad<ubit32_t, ubit1_t, ubit1_t> result = add_with_carry<ubit32_t, 32>(GET_R(rdnx, machine.arm), shifted, G_CF(machine.arm) ? 1 : 0);

    GET_R(rdnx) = result.item1;
    if(!in_it_block(machine)) {
        mask_bit<ubit1_t, ubit32_t, 31>(result.item1) == 1 ? S_NF(machine.arm) : C_NF(machine.arm);
        result.item1 == 0 ? S_ZF(machine.arm) : C_ZF(machine.arm);
        result.item2 == 1 ? S_CF(machine.arm) : C_CF(machine.arm);
        result.item3 == 1 ? S_VF(machine.arm) : C_VF(machine.arm);
    }
}

void adc_reg_t2(ubit16_t opcode_h, ubit16_t opcode_l, machine_t& machine) {
    ubit4_t dx = mask_bits<ubit4_t, ubit16_t, 8, 4>(opcode_l);
    ubit4_t nx = mask_bits<ubit4_t, ubit16_t, 0, 4>(opcode_h);
    ubit4_t mx = mask_bits<ubit4_t, ubit16_t, 0, 4>(opcode_l);
    couple<sr_type, ubit8_t> shift_couple = decode_imm_shift(mask_bits<ubit2_t, ubit16_t, 4, 2>(opcode_l), 
        (mask_bits<ubit8_t, ubit16_t, 12, 3>(opcode_l) << 2) | mask_bit<ubit8_t, ubit16_t, 6, 2>(opcode_l));
    
    if(BAD_R(dx, machine.arm) || BAD_R(nx, machine.arm) || BAD_R(mx, machine.arm)) { /*unpredicttable*/ return ; }

    ubit32_t shifted = shift<ubit32_t, 32)(GET_R(mx, machine.arm), shift_couple.item1, shift_couple.item2, G_CF(machine.arm) ? 1 : 0);
    triad<ubit32_t, ubit1_t, ubit1_t> result = add_with_carry<ubit32_t, 32)(GET_R(nx, machine.arm), shifted, G_CF(machine.arm) ? 1 : 0);
    GET_R(dx) = result.item1;
    if(mask_bit<ubit1_t, ubit16_t, 4>(opcode_h) == 1) {
        mask_bit<ubit1_t, ubit32_t, 31>(result.item1) == 1 ? S_NF(machine.arm) : C_NF(machine.arm);
        result.item1 == 0 ? S_ZF(machine.arm) : C_ZF(machine.arm);
        result.item2 == 1 ? S_CF(machine.arm) : C_CF(machine.arm);
        result.item3 == 1 ? S_VF(machine.arm) : C_VF(machine.arm);
    }
}