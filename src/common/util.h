#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef uint32_t word;
typedef uint8_t byte;
typedef unsigned int uint;

typedef enum { ERROR_SUCC = 0, ERROR_FAIL = 1 } Error;
/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
typedef enum { PC = 15, CPSR = 16 } Register;
// opcode mnemonics for DPI
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
// opcode mnemonics for Multiply Instructions
typedef enum { MUL, MLA } OpcodeMultiply;
// opcode mnemonics for SDTI
typedef enum { LDR, STR } OpcodeSDT;
// opcode mnemonics for Branch Instructions
typedef enum { BEQ, BNE, BGE, BLT, BGT, BLE, B } OpcodeBranch;
// opcode mnemonics for Special Instructions
typedef enum { LSL_SPECIAL = 14, ANDEQ } OpcodeSpecial;
// condition suffixes
typedef enum { EQ, NE, GE = 10, LT, GT, LE, AL } Cond;
// shift types
typedef enum { LSL, LSR, ASR, ROR } Shift;

extern char *strptr(const char *in);
extern Error validatePtr(const void *ptr, const char *errorMsg);
extern void errorExit(Error error);

#endif
