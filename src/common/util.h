#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };
// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// condition suffixes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };
// shift types
enum Shift { LSL, LSR, ASR, ROR };
typedef enum { ERR_PASS = 0, ERR_FAIL = 1 } Error;

typedef uint32_t word;
typedef uint8_t byte;
typedef unsigned int uint;

extern char *strptr(char *in);
extern int validatePtr(const void *ptr, const char *error_msg);
extern void errorExit(Error error);

#endif
