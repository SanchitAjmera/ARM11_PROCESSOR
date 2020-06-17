#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef uint32_t word;
typedef uint8_t byte;
typedef unsigned int uint;
// Enums for checked errors within program
typedef enum {
  ERROR_SUCC,
  ERR_FAIL,
  MEM_ASSIGN,
  MEM_OVERFLOW,
  FILE_ERR,
  UNEXPECTED_CASE,
  UNEXPECTED_ARGS
} Error;
// Enum registers for special named registers
typedef enum { SP = 13, LR, PC, CPSR } Register;
// Opcode mnemonics for DPI
typedef enum {
  AND,
  EOR,
  SUB,
  RSB,
  ADD,
  TST = 8,
  TEQ,
  CMP,
  ORR = 12,
  MOV
} OpcodeDP;
// Opcode mnemonics for Multiply Instructions
typedef enum { MUL, MLA } OpcodeMultiply;
// Opcode mnemonics for SDTI
typedef enum { LDR, STR } OpcodeSDT;
// Opcode mnemonics for Branch Instructions
typedef enum { BEQ, BNE, BGE, BLT, BGT, BLE, B } OpcodeBranch;
// Opcode mnemonics for Special Instructions
typedef enum { LSL_SPECIAL = 15, ANDEQ } OpcodeSpecial;
// Condition suffixes
typedef enum { EQ, NE, GE = 10, LT, GT, LE, AL } Cond;
// Shift types
typedef enum { LSL, LSR, ASR, ROR } Shift;

extern char *strptr(const char *in);
extern Error validatePtr(const void *ptr);
extern void errorExit(Error error);

#endif
