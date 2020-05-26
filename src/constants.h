#ifndef

// general constants
#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

// Constants for Multiply
#define SDTI_I_MASK 0x02000000
#define SDTI_P_MASK 0x01000000
#define SDTI_U_MASK 0x00800000
#define SDTI_L_MASK 0x00100000
#define SDTI_RN_MASK 0x000F0000
#define SDTI_RD_MASK 0x0000F000
#define SDTI_OFFSET_MASK 0x00000FFF
#define SDTI_I_SHIFT 25
#define MULT_P_SHIFT 24
#define MULT_U_SHIFT 23
#define MULT_L_SHIFT 20
#define MULT_RN_SHIFT 16
#define MULT_RD_SHIFT 12

#endif
