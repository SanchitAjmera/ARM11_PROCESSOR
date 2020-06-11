#ifndef EMULATE_CONSTANTS_H
#define EMULATE_CONSTANTS_H

// general constants
#define NOT_BIG_ENDIAN false
#define IS_BIG_ENDIAN true
#define CPSR_N_MASK (1 << 31)
#define CPSR_Z_MASK (1 << 30)
#define CPSR_C_MASK (1 << 29)
#define CPSR_V_MASK (1 << 28)
#define CPSR_N_SHIFT 31
#define CPSR_Z_SHIFT 30
#define CPSR_C_SHIFT 29
#define CPSR_V_SHIFT 28
#define GET_CPSR_N(cpsr) ((cpsr & CPSR_N_MASK) >> CPSR_N_SHIFT)
#define GET_CPSR_Z(cpsr) ((cpsr & CPSR_Z_MASK) >> CPSR_Z_SHIFT)
#define GET_CPSR_C(cpsr) ((cpsr & CPSR_C_MASK) >> CPSR_C_SHIFT)
#define GET_CPSR_V(cpsr) ((cpsr & CPSR_V_MASK) >> CPSR_V_SHIFT)
#define GET_CPSR_FLAGS(cpsr) (cpsr >> CPSR_V_SHIFT)
#define UPDATE_CPSR(instruction) (instruction & INSTRUCTION_S_MASK)
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

// constants for decode function
#define DECODE_DPI_MASK 0x0C000000
#define DECODE_DPI_EXPECTED 0x00000000
#define DECODE_MULT_MASK 0x0FC000F0
#define DECODE_MULT_EXPECTED 0x0000090
#define DECODE_SDT_MASK 0x0C600000
#define DECODE_SDT_EXPECTED 0x04000000
#define DECODE_BRANCH_MASK 0x0F000000
#define DECODE_BRANCH_EXPECTED 0x0A000000

// constants for single data transfer
#define SDTI_I_MASK 0x02000000
#define SDTI_P_MASK 0x01000000
#define SDTI_U_MASK 0x00800000
#define SDTI_L_MASK 0x00100000
#define SDTI_RN_MASK 0x000F0000
#define SDTI_RD_MASK 0x0000F000
#define SDTI_OFFSET_MASK 0x00000FFF

// constants for Multiply instruction
#define MULT_REG_M_MASK 0x0000000F
#define INSTRUCTION_S_MASK (1 << 20)

// constants for Branch instruction
#define BRANCH_SIGN_BIT (1 << 23)
#define CURRENT_INSTRUCTION_SHIFT 2
#define NEGATIVE_SIGN_EXTEND 0xFC000000
#define POSITIVE_SIGN_EXTEND 0

// constants for Data Processing instruction
#define SET_CPSR_C (1 << 29)
#define DPI_I_MASK 0x02000000
#define DPI_OPCODE_MASK 0x01E00000
#define DPI_RN_MASK 0xF0000
#define DPI_RD_MASK 0xF000
#define DPI_OP2_MASK 0x00000FFF
#define LEAST_BYTE_MASK 0xFF
#define GET_ROTATION_NUM 8
#define GET_RS_SHIFT 4
#define SHIFT_TYPE_MASK 0x06
#define GET_SHIFT_TYPE 1
#define LEAST_NIBBLE_MASK 0xF
#define GET_SHIFT_INSTRUCTION 4
#define GET_SHIFT_CONSTANT_SHIFT 3
#define MSB_MASK 1 << 31
#define LSB_MASK 0x1
#define NO_ROTATION 0
#define IS_ADDITION true
#define NOT_ADDITION false
#endif
