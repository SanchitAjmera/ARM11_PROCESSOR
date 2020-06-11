#ifndef ASSEMBLE_CONSTANTS_H
#define ASSEMBLE_CONSTANTS_H

// general constants & pre-processor functions
#define IS_IMMEDIATE(op) (op[0] == '#')
#define MAX_NUM (4294967295)
// TODO: MAX_NUM (pow(2, WORD_SIZE) - 1)
#define OVERFLOW(num) (num > MAX_NUM)
#define LOOKUP_FAILURE (-1)
#define LOOKUP_FAILED(num) (num == LOOKUP_FAILURE)

// constants for Data Processing Instructions
#define MAX_BYTE (0xFF)
#define HEX_BASE (16)
#define DPI_I_SHIFT 25
#define DPI_OPCODE_SHIFT 21
#define DPI_RN_SHIFT 16
#define DPI_RD_SHIFT 12
#define DPI_S_SHIFT 20
#define SHIFT_TABLE_SIZE (4)
#define OPCODE_TABLE_SIZE (10)

// constants for Single Data Transfer Instructions

// constants for Multiply Instructions

// constants for Branch Instructions

#define COND_TABLE_SIZE (7)

#endif
