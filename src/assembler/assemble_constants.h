#ifndef ASSEMBLE_CONSTANTS_H
#define ASSEMBLE_CONSTANTS_H

#include <stdlib.h>

// Universal constants & pre-processor functions
#define MAX_NUM (4294967295)
// TODO: MAX_NUM (pow(2, WORD_SIZE) - 1)
#define OVERFLOW(num) (num > MAX_NUM)
#define LOOKUP_FAILURE (-1)
#define LOOKUP_FAILED(num) (num == LOOKUP_FAILURE)
#define IS_IMMEDIATE(exp) (exp[0] == '#' || exp[0] == '=')
#define REMOVE_FIRST_CHAR(string) (++string)
#define REM_INT(string) (atoi(REMOVE_FIRST_CHAR(string)))

// Constants for File Lines
#define INIT_FILE_LINES_SIZE (16)

// Constants for Symbol Table
#define INIT_S_TABLE_SIZE (32)
#define LOAD_FACTOR (0.75)
#define PRIME_INIT (7)
#define PRIME_FACTOR (31)

// constants for Data Processing Instructions
#define MAX_BYTE (0xFF)
#define HEX_BASE (16)
#define DPI_I_SHIFT (25)
#define DPI_OPCODE_SHIFT (21)
#define DPI_RN_SHIFT (16)
#define DPI_RD_SHIFT (12)
#define DPI_S_SHIFT (20)
#define SHIFT_BY_REG_HARDCODE (1 << 4)
#define RS_SHIFT (8)
#define SHIFT_NUM_SHIFT (7)
#define SHIFT_TYPE_SHIFT (5)
#define SHIFT_NO_ARGS (3)
#define SHIFT_TABLE_SIZE (4)
#define OPCODE_TABLE_SIZE (12)

// constants for Single Data Transfer Instructions

// constants for Multiply Instructions

// constants for Branch Instructions

#define COND_TABLE_SIZE (7)

#endif
