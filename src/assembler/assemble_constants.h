#ifndef ASSEMBLE_CONSTANTS_H
#define ASSEMBLE_CONSTANTS_H

// general constants & pre-processor functions
#define IS_IMMEDIATE(exp) (exp[0] == '#' || exp[0] == '=')
#define REMOVE_FIRST_CHAR(string) (++string)
#define REM_INT(string) (atoi(REMOVE_FIRST_CHAR(string)))
#define MAX_NUM (4294967295)
#define MAX_BYTE (0xFF)
#define LOOKUP_FAILURE (-1)
#define LOOKUP_FAILED(num) (num == LOOKUP_FAILURE)
#define LINE_CHAR_LIM (512)
#define PREDEFINED_SYMBOLS_COUNT 23

// constants for File Lines
#define INIT_FILE_LINES_SIZE (16)

// constants for Symbol Table
#define INIT_S_TABLE_SIZE (32)

// constants for Data Processing Instructions
#define HEX_BASE (16)
#define DPI_S_SHIFT (20)
#define SHIFT_BY_REG_HARDCODE (1 << 4)
#define RS_SHIFT (8)
#define SHIFT_TYPE_SHIFT (5)
#define SHIFT_NO_ARGS (3)
#define SHIFT_TABLE_SIZE (4)
#define OPCODE_TABLE_SIZE (12)

// constants for Single Data Transfer Instructions
#define SDTI_HARDCODE (1 << 26)
#define SDTI_EXP_BOUND (0xFF)

// constants for Multiply Instructions
#define MULT_HARDCODE (0x90)

// constants for Branch Instructions
#define BRANCH_HARDCODE (0x0A000000)
#define COND_TABLE_SIZE (7)

#endif
