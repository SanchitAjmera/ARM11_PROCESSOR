#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef uint32_t word;
typedef uint8_t byte;
typedef unsigned int uint;
// Enums for checked errors within program
typedef enum { ERROR_SUCC, ERR_FAIL, ERR_PASS } Error;
// Enum registers for special named registers
typedef enum { PC = 15, CPSR = 16 } Register;
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

typedef struct {
  char *value;
  int length, maxLength;
} resizable_string;

extern char *strptr(const char *in);
extern Error validatePtr(const void *ptr, const char *errorMsg);
extern void errorExit(Error error);
extern void extendString(resizable_string *string);
extern resizable_string *newString(void);
extern void appendToString(resizable_string *string, const char *append);
extern void freeString(resizable_string *string);

#endif
