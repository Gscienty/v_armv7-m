#ifndef _CPU_
#define _CPU_

#include "..\global.h"

#define M_USR 0x01
#define M_FIQ 0x02
#define M_IRQ 0x03
#define M_SVC 0x04
#define M_ABT 0x05
#define M_UND 0x06
#define M_SYS 0x07

#define NF  0x80000000
#define ZF  0x40000000
#define CF  0x20000000
#define VF  0x10000000
#define QF  0x08000000
#define JF  0x01000000
#define GE3 0x00080000
#define GE2 0x00040000
#define GE1 0x00020000
#define GE0 0x00010000
#define GE  0x000F0000
#define AF  0x00000100
#define EF  0x00000200
#define IF  0x00000080
#define FF  0x00000040
#define TF  0x00000020
#define M4F 0x00000010
#define M3F 0x00000008
#define M2F 0x00000004
#define M1F 0x00000002
#define M0F 0x00000001
#define MF  0x0000001F

#define E_RESET 
#define E_UNDEF
#define E_SOFTI
#define E_PREAB
#define E_IRO
#define E_FIQ

struct arm_t {
    ubit32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
    ubit32_t cpsr;
    ubit32_t spsr_svc, spsr_abt, spsr_und, spsr_irq, spsr_fiq;
    ubit32_t r13_svc, r13_abt, r13_und, r13_irq;
    ubit32_t r14_svc, r14_abt, r14_und, r14_irq;
    ubit32_t r8_fiq, r9_fiq, r10_fiq, r11_fiq, r12_fiq, r13_fiq, r14_fiq;
};

#define GET_MODE(bin)  (bin == binary<10000>::val ? M_USR : \
                        bin == binary<10001>::val ? M_FIQ : \
                        bin == binary<10010>::val ? M_IRQ : \
                        bin == binary<10011>::val ? M_SVC : \
                        bin == binary<10111>::val ? M_ABT : \
                        bin == binary<11011>::val ? M_UND : \
                                                    M_SYS   )

#define G_MODE(arm)   (arm.cpsr & MF)
#define G_TF(arm) (!!(arm.cpsr & TF))
#define G_FF(arm) (!!(arm.cpsr & FF))
#define G_IF(arm) (!!(arm.cpsr & IF))
#define G_AF(arm) (!!(arm.cpsr & AF))
#define G_EF(arm) (!!(arm.cpsr & EF))
#define G_GE(arm) ((arm.cpsr & GE) >> 16)
#define G_JF(arm) (!!(arm.cpsr & JF))
#define G_QF(arm) (!!(arm.cpsr & QF))
#define G_VF(arm) (!!(arm.cpsr & VF))
#define G_CF(arm) (!!(arm.cpsr & CF))
#define G_ZF(arm) (!!(arm.cpsr & ZF))
#define G_NF(arm) (!!(arm.cpsr & NF))

#define S_MODE_USR(arm) (arm.cpsr |= binary<10000>::val)
#define S_MODE_FIQ(arm) (arm.cpsr |= binary<10001>::val)
#define S_MODE_IRQ(arm) (arm.cpsr |= binary<10010>::val)
#define S_MODE_SVC(arm) (arm.cpsr |= binary<10011>::val)
#define S_MODE_ABT(arm) (arm.cpsr |= binary<10111>::val)
#define S_MODE_UND(arm) (arm.cpsr |= binary<11011>::val)
#define S_MODE_SYS(arm) (arm.cpsr |= binary<11111>::val)
#define S_GE(arm, val) (arm.cpsr |= val << 16)
#define S_TF(arm) (arm.cpsr |= TF)
#define S_FF(arm) (arm.cpsr |= FF)
#define S_IF(arm) (arm.cpsr |= IF)
#define S_AF(arm) (arm.cpsr |= AF)
#define S_EF(arm) (arm.cpsr |= EF)
#define S_JF(arm) (arm.cpsr |= JF)
#define S_QF(arm) (arm.cpsr |= QF)
#define S_VF(arm) (arm.cpsr |= VF)
#define S_CF(arm) (arm.cpsr |= CF)
#define S_ZF(arm) (arm.cpsr |= ZF)
#define S_NF(arm) (arm.cpsr |= NF)
#define C_TF(arm) (arm.cpsr &= ~TF)
#define C_FF(arm) (arm.cpsr &= ~FF)
#define C_IF(arm) (arm.cpsr &= ~IF)
#define C_AF(arm) (arm.cpsr &= ~AF)
#define C_EF(arm) (arm.cpsr &= ~EF)
#define C_JF(arm) (arm.cpsr &= ~JF)
#define C_QF(arm) (arm.cpsr &= ~QF)
#define C_VF(arm) (arm.cpsr &= ~VF)
#define C_CF(arm) (arm.cpsr &= ~CF)
#define C_ZF(arm) (arm.cpsr &= ~ZF)
#define C_NF(arm) (arm.cpsr &= ~NF)
#define C_MF(arm) (arm.cpsr &= ~MF)

#define RW(r) (*(ubyte4_t *)&r)
#define RH(r) (*(ubyte2_t *)&r)
#define RB(r) (*(ubyte1_t *)&r)

#define R0( arm) (arm.r0)
#define R1( arm) (arm.r1)
#define R2( arm) (arm.r2)
#define R3( arm) (arm.r3)
#define R4( arm) (arm.r4)
#define R5( arm) (arm.r5)
#define R6( arm) (arm.r6)
#define R7( arm) (arm.r7)
#define R8( arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.r8_fiq  : arm.r8 )
#define R9( arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.r9_fiq  : arm.r9 )
#define R10(arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.r10_fiq : arm.r10)
#define R11(arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.r11_fiq : arm.r11)
#define R12(arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.r12_fiq : arm.r12)
#define R13(arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.r13_fiq : \
                  M_IRQ == GET_MODE(arm.cpsr) ? arm.r13_irq : \
                  M_SVC == GET_MODE(arm.cpsr) ? arm.r13_svc : \
                  M_UND == GET_MODE(arm.cpsr) ? arm.r13_und : \
                  M_ABT == GET_MODE(arm.cpsr) ? arm.r13_abt : \
                                                arm.r13       )
#define R14(arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.r14_fiq : \
                  M_IRQ == GET_MODE(arm.cpsr) ? arm.r14_irq : \
                  M_SVC == GET_MODE(arm.cpsr) ? arm.r14_svc : \
                  M_UND == GET_MODE(arm.cpsr) ? arm.r14_und : \
                  M_ABT == GET_MODE(arm.cpsr) ? arm.r14_abt : \
                                                arm.r14       )
#define SPSR(arm) (M_FIQ == GET_MODE(arm.cpsr) ? arm.spsr_fiq : \
                   M_IRQ == GET_MODE(arm.cpsr) ? arm.spsr_irq : \
                   M_SVC == GET_MODE(arm.cpsr) ? arm.spsr_svc : \
                   M_UND == GET_MODE(arm.cpsr) ? arm.spsr_und : \
                                                 arm.spsr_abt   )


#define GET_R(n, arm)  (n == 0  ? R0(arm)  : \
                        n == 1  ? R1(arm)  : \
                        n == 2  ? R2(arm)  : \
                        n == 3  ? R3(arm)  : \
                        n == 4  ? R4(arm)  : \
                        n == 5  ? R5(arm)  : \
                        n == 6  ? R6(arm)  : \
                        n == 7  ? R7(arm)  : \
                        n == 8  ? R8(arm)  : \
                        n == 9  ? R9(arm)  : \
                        n == 10 ? R10(arm) : \
                        n == 11 ? R11(arm) : \
                        n == 12 ? R12(arm) : \
                        n == 13 ? R13(arm) : \
                        n == 14 ? R14(arm) : SPSR(arm))

#define EQ    0x00
#define NE    0x01
#define CS_HS 0x02
#define CC_LO 0x03
#define MI    0x04
#define PL    0x05
#define VS    0x06
#define VC    0x07
#define HI    0x08
#define LS    0x09
#define GE    0x0A
#define LT    0x0B
#define GT    0x0C
#define LE    0x0D
#define AL    0x0E

#define CONDINS(cond, arm) (cond == EQ    ?   G_ZF(arm) : \
                            cond == NE    ?  !G_ZF(arm) : \
                            cond == CS_HS ?   G_CF(arm) : \
                            cond == CC_LO ?  !G_CF(arm) : \
                            cond == MI    ?   G_NF(arm) : \
                            cond == PL    ?  !G_NF(arm) : \
                            cond == VS    ?   G_VF(arm) : \
                            cond == VC    ?  !G_VF(arm) : \
                            cond == HI    ?  !G_ZF(arm) && G_CF(arm) : \
                            cond == LS    ?  !G_CF(arm) || G_ZF(arm) : \
                            cond == GE    ? ( G_NF(arm) && G_VF(arm)) || (!G_NF(arm) && !G_VF(arm)) : \
                            cond == LT    ? (!G_VF(arm) && G_NF(arm)) || (!G_NF(arm) &&  G_VF(arm)) : \
                            cond == GT    ? (!G_ZF(arm) && G_NF(arm) && G_VF(arm)) || (!G_ZF(arm) && !G_NF(arm) && !G_VF(arm)) : \
                            cond == LE    ?   G_ZF(arm) || (G_NF(arm) && !G_VF(arm)) || (!G_NF(arm) && G_VF(arm)) : \
                                            true)

#define BAD_R(r, ram) (r == 13 || r == 15)

#endif