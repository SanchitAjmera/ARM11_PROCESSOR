#ifndef EMULATE_UTIL_H
#define EMULATE_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint32_t word;
typedef uint8_t byte;
typedef unsigned int uint;

// TODO: consider changing the type of carryOut
typedef struct {
  word result;
  word carryOut;
} operation_t;

// tuple for instruction and instructionSet enum
typedef struct {
  bool is_set;
  word instr;
  InstructionSet instrSet;
} tuple_instruction;

typedef struct {
  byte *memory;
  /* 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR */
  word *registers;
  word fetched;
  tuple_instruction decoded;
} arm;

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
typedef enum { DPI, MULT, BR, SDTI, IGNR } InstructionSet;


extern void check_ptr(const void *ptr, const char *error_msg);

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
extern void init_arm(arm *state, const char *fname);

// execution of the multiply instruction
extern void multiply(arm *state, word instruction);

// execution of a branch instruction
extern void branch(arm *state, word instruction);

extern word get_word(byte *start_addr);
word get_word_big_end(byte *start_addr);
extern void fetch(arm *state);
extern void decode(arm *state);
extern void execute(arm *state);

#endif
