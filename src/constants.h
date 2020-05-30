#ifndef CONSTANTS_H
#define CONSTANTS_H

// general constants
#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define WORD_SIZE 32
#define MSB_MASK 1 << 31
#define LSB_MASK 0x1
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

// constants for Multiply instruction
#define MULT_RDEST_MASK 0x000F0000
#define MULT_REG_S_MASK 0x00000F00
#define MULT_REG_M_MASK 0x0000000F
#define MULT_REG_N_MASK 0x0000F000
#define MULT_REG_S_SHIFT 8
#define MULT_REG_N_SHIFT 12
#define MULT_RDEST_SHIFT 16
#define ACCUMULATE_FLAG (1 << 21)
#define UPDATE_CPSR (1 << 20)
#define CPSR_N (1 << 31)
#define CPSR_Z (1 << 30)

// constants for Branch instruction
#define BRANCH_OFFSET_MASK 0x00FFFFFF
#define BRANCH_SIGN_BIT (1 << 23)
#define CURRENT_INSTRUCTION_SHIFT 2
#define NEGATIVE_SIGN_EXTEND 0xFC000000
#define POSITIVE_SIGN_EXTEND 0

// constants for Data Processing instruction
#define TWOS_COMPLEMENT(value) ((~value) + 1)
#define CPSR_V_MASK 0x10000000
#define SET_CPSR_C (1 << 29)
#define DPI_I_MASK 0x02000000
#define DPI_OPCODE_MASK 0x01E00000
#define DPI_S_MASK 0x00100000
#define DPI_RN_MASK 0xF0000
#define DPI_RD_MASK 0xF000
#define DPI_OP2_MASK 0x00000FFF
#define DPI_I_SHIFT 25
#define DPI_OPCODE_SHIFT 21
#define DPI_S_SHIFT 20
#define DPI_RN_SHIFT 16
#define DPI_RD_SHIFT 12
#define LEAST_BYTE 0xFF
#define GET_ROTATE_SHIFT 8
#define ROTATION_FACTOR 2
#define GET_RS_SHIFT 4
#define SHIFT_TYPE_MASK 0x06
#define GET_SHIFT_TYPE_SHIFT 1
#define LSN_MASK 0xF
#define GET_SHIFT_INSTRUCTION_SHIFT 4
#define GET_SHIFT_CONSTANT_SHIFT 3
#endif
