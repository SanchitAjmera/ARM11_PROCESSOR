#ifndef EMULATE_UTIL_H
#define EMULATE_UTIL_H

#include "include/constants.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };
// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// condition suffixes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };
// shift types
enum Shift { LSL, LSR, ASR, ROR };
// ARM instruction set
typedef enum { DPI, MULT, BR, SDTI, IGNR } InstructionType;

typedef uint8_t byte;

// struct for the result and carry out from shift/arithmetic operation
typedef struct {
  word result;
  uint carryOut;
} operation_t;

// struct for the extraction of Data Processing instructions
typedef struct {
  word instruction;
  uint i;
  enum Opcode opcode;
  uint rn;
  uint rd;
  word op1;
  word op2;
} dpi;

// struct for instruction and InstructionType enum
typedef struct {
  bool isSet;
  word instruction;
  InstructionType instructionType;
} instructionState;

typedef struct {
  byte *memory;
  /* 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR */
  word *registers;
  word fetched;
  instructionState decoded;
} arm;

extern void validatePtr(const void *ptr, const char *error_msg);

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
extern void initArm(arm *state, const char *fname);

extern word getWord(byte *start_addr, bool isBigEndian);
extern void fetch(arm *state);
extern void decode(arm *state);
extern void execute(arm *state);

#endif
