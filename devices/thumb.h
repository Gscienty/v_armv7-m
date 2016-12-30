#ifndef _THUMB_
#define _THUMB_
#include "machine.h"

couple<ubit32_t, ubit1_t> thumb_expand_imm_c(ubit12_t imm12, ubit1_t carry_in);
ubit32_t thumb_expand_imm(ubit12_t imm12, bool apsr_cf);

void adc_imm(ubit16_t opcode_h, ubit16_t opcode_l, machine_t& machine);
void adc_reg_t1(ubit16_t opcode, machine_t& machine);
void adc_reg_t2(ubit16_t opcode_h, ubit16_t opcode_l, machine_t& machine);
#endif