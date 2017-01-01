#ifndef _GLOBAL_
#define _GLOBAL_

typedef unsigned char      ubit1_t ;
typedef unsigned char      ubit2_t ;
typedef unsigned char      ubit4_t ;
typedef unsigned char      ubit8_t ;
typedef unsigned short     ubit12_t;
typedef unsigned short     ubit16_t;
typedef unsigned int       ubit32_t;
typedef unsigned long      ubit64_t;
typedef unsigned __int128 ubit128_t;

typedef signed char      bit8_t;
typedef signed short     bit16_t;
typedef signed int       bit32_t;
typedef signed long      bit64_t;
typedef signed __int128 bit128_t;

enum sr_type { none, lsl, lsr, asr, ror, rrx };

#define CV(t, v) (*(t *)&v)

#define ASSERT(cond) if(!(cond)) { throw ; } else 

template<typename T> struct get_bit { template<T a, T b> struct operands { static const bool val = !!(a & b); }; };
template<typename T> struct set_bit { template<T a, T b> struct operands { static const T    val = a | b;     }; };

template<ubit128_t b> struct binary { static const ubit32_t val = (b & 0x01) | ((binary<(b / 10)>::val) << 1); };
template<> struct binary<0> { static const ubit32_t val = 0; };
template<> struct binary<1> { static const ubit32_t val = 1; };

template<typename T1, typename T2> struct couple { T1 item1; T2 item2; }; 
template<typename T1, typename T2, typename T3> struct triad { T1 item1; T2 item2; T3 item3; };

template<typename T, ubit8_t n> struct sign_limit { static const T max = (1 << (n - 1)) - 1; static const T min = -(1 << (n - 1)); };

//utilities function

template<typename TO, typename TI, ubit8_t start, ubit8_t len> TO mask_bits (TI origin_bit) {
    TI mask = 0x01;
    for(ubit8_t loop = 0; loop < len - 1; loop++) { mask <<= 1; mask |= 0x01; }
    return (TO)((origin_bit & (mask << start)) >> start);
}

template<typename TO, typename TI, ubit8_t index> TO mask_bit(TI origin_bit) { return mask_bits<TO, TI, index, 1>(origin_bit); }

template<typename T, ubit8_t n> couple<T, ubit1_t> SL_C(T x, ubit8_t shift) { ASSERT(shift > 0) 
    { T middle = x << (shift - 1); return { mask_bits<T, T, 0, n>(middle << 1), mask_bit<ubit1_t, T, n - 1>(middle) }; }
}

template<typename T, ubit8_t n> T SL(T x, ubit8_t shift) { ASSERT(shift >= 0) 
    if (shift == 0) { return mask_bits<T, T, 0, n>(x); } else { return SL_C<T, n>(x, shift).item1; } 
}

template<typename T, ubit8_t n> couple<T, ubit1_t> SR_C(T x, ubit8_t shift) { ASSERT(shift > 0) 
    { T middle = x >> (shift - 1); return { mask_bits<T, T, 0, n>(middle >> 1), mask_bit<ubit1_t, T, 0>(middle) }; }
}

template<typename T, ubit8_t n> T SR(T x, ubit8_t shift) { ASSERT(shift >= 0) 
    if (shift == 0) { return mask_bits<T, T, 0, n>(x); } else { return SR_C<T, n>(x, shift).item1; }
}

template<typename T, ubit8_t n> couple<T, ubit8_t> ROR_C(T x, ubit8_t shift) { ASSERT(shift != 0) 
    { ubit8_t m = shift % n; T result = SR<T, n>(x, m) | SL<T, n>(x, n - m); return { mask_bits<T, T, 0, n>(result), mask_bit<ubit1_t, T, n - 1>(result) }; }
}

template<typename T, ubit8_t n> T ROR(T x, ubit8_t shift) { 
    if(shift == 0) { return mask_bits<T, T>(x, 0, n - 1); } else { return ROR_C<T, n>(x, shift).item1; } 
}

template<typename T, ubit8_t n> couple<T, ubit1_t> RRX_C(T x, ubit1_t carry_in) { 
    return { mask_bits<T, T, 0, n>((carry_in << (n - 1)) | mask_bits<T, T, 1, n - 1>(x)), mask_bit<ubit1_t, T, 0>(x) }; 
}

template<typename T, ubit8_t n> T RRX(T x, ubit1_t carry_in) { return RRX_C<T, n>(x, carry_in).item1; }

template<typename T, ubit8_t n> triad<T, ubit1_t, ubit1_t> add_with_carry(T x, T y, ubit1_t carry_in) {
    T middle = x + y + carry_in;
    return { 
        middle, 
        middle < x || middle < y,
        mask_bit<ubit1_t, T, n - 1>(x) == mask_bit<ubit1_t, T, n - 1>(y) && mask_bit<ubit1_t, T, n - 1>(x) != mask_bit<ubit1_t, T, n - 1>(middle)
    };
}

template<typename TO, typename TI> TO zero_extend(const TI in_param) { return (TO)in_param; }

#define ALIGN(x, y) ((y) * ((x) / (y)))
#endif