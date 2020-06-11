#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef enum { ERR_PASS = 0, ERR_FAIL = 1 } Error;
/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
typedef enum { PC = 15, CPSR = 16 } Register;
// opcode mnemonics
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
  MOV,
  ANDEQ,
  LSL_SPECIAL,
} Opcode;
// condition suffixes
typedef enum { EQ, NE, GE = 10, LT, GT, LE, AL } Cond;
// shift types
typedef enum { LSL, LSR, ASR, ROR } Shift;

typedef uint32_t word;
typedef uint8_t byte;
typedef unsigned int uint;

extern char *strptr(char *in);
extern Error validatePtr(const void *ptr, const char *errorMsg);
extern void errorExit(Error error);

#endif
