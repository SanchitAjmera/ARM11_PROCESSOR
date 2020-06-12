#ifndef CONSTANTS_H
#define CONSTANTS_H

// Universal constants
#define MEMORY_CAPACITY (65536)
#define NUM_REGISTERS (17)
#define WORD_SIZE (32)
#define ADDRESS_SIZE (4)
#define WORD_SIZE_BYTES (4)
#define BYTE (8)
#define COND_SHIFT (28)

// Constants for Data Processing Instructions
#define ROTATION_FACTOR (2)
#define DPI_I_SHIFT (25)
#define DPI_OPCODE_SHIFT (21)
#define DPI_RN_SHIFT (16)
#define DPI_RD_SHIFT (12)
#define GET_ROTATION_NUM (8)

// Constants for Single Data Transfer Instructions
#define SDTI_I_SHIFT (25)
#define SDTI_P_SHIFT (24)
#define SDTI_U_SHIFT (23)
#define SDTI_L_SHIFT (20)
#define SDTI_RN_SHIFT (16)
#define SDTI_RD_SHIFT (12)

// Constants for Multiply Instructions
#define MULT_RDEST_MASK (0x000F0000)
#define MULT_REG_S_MASK (0x00000F00)
#define MULT_REG_N_MASK (0x0000F000)
#define MULT_REG_S_SHIFT (8)
#define MULT_REG_N_SHIFT (12)
#define MULT_RDEST_SHIFT (16)
#define ACCUMULATE_FLAG (1 << 21)

// Constants for Branch Instructions
#define BRANCH_OFFSET_MASK (0x00FFFFFF)
#define CURRENT_INSTRUCTION_SHIFT (2)

#endif
