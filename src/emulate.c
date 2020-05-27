#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
// Constants will be moved into a seperate header file
// general constants
#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

// Constants for Branch
#define BRANCH_OFFSET_MASK 0x00FFFFFF
#define BRANCH_SIGN_BIT (1 << 23)
#define CURRENT_INSTRUCTION_SHIFT 2
#define NEGATIVE_SIGN_EXTEND 0xFC000000
#define POSITIVE_SIGN_EXTEND 0

typedef uint32_t word;

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };
// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// condition suffixes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };
// shift types
enum Shift { LSL, LSR, ASR, ROR };

typedef struct {
  // ARM machine memory
  uint8_t *memory;
  // 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR
  word *registers;
} arm;

void ptrValidate(const void *pointer, char *error) {
  if (pointer == NULL) {
    printf("Error: %s\n", error);
    exit(EXIT_FAILURE);
  }
}

// From Alex with shifts included
bool checkCond(arm *state, word instruction) {
  // CPSR flag bits
  unsigned int n = (state->registers[CPSR] & 0x80000000) >> 31;
  unsigned int z = (state->registers[CPSR] & 0x40000000) >> 30;
  unsigned int c = (state->registers[CPSR] & 0x20000000) >> 29;
  unsigned int v = (state->registers[CPSR] & 0x10000000) >> 28;
  unsigned int cond = instruction >> 28;
  // conditions for instruction
  switch (cond) {
  case EQ:
    return z;
  case NE:
    return !z;
  case GE:
    return n == v;
  case LT:
    return n != v;
  case GT:
    return !z && (n == v);
  case LE:
    return z || (n != v);
  case AL:
    return true;
  default:
    return false;
  }
}

void branch(arm *state, word instruction) {
  // no execution if condition code does not match
  if (!checkCond(state, instruction))
    return;

  // Extraction of information
  int offset = instruction & BRANCH_OFFSET_MASK;
  int signBit = offset & BRANCH_SIGN_BIT;

  // Shift, sign extension and addition of offset onto current address
  state->registers[PC] +=
      (offset << CURRENT_INSTRUCTION_SHIFT) |
      (signBit ? NEGATIVE_SIGN_EXTEND : POSITIVE_SIGN_EXTEND);
}

void decode(arm state, word instruction) {
  const word dpMask = 0x0C000000;
  const word dp = 0x00000000;
  const word multMask = 0x0FC000F0;
  const word mult = 0x0000090;
  const word sdtMask = 0x0C600000;
  const word sdt = 0x04000000;
  const word branchMask = 0x0F000000;
  const word branch = 0x0A000000;

  // TODO: determine how to differentiate ...
  // ... `data processing` from `multiply`

  if (BITS_SET(instruction, branchMask, branch)) {
    // function for branch instructions
  } else if (BITS_SET(instruction, sdtMask, sdt)) {
    // function for single data tranfser instructions
  } else if (BITS_SET(instruction, multMask, mult)) {
    // function for multiply instructions
  } else if (BITS_SET(instruction, dpMask, dp)) {
    // function for data processing instructions
  }
}

=======
#include "emulate_util.h"

>>>>>>> 3ae83d21e1d3c9d83ae4bb2cd92ba566686f12d8
int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Please specify an ARM binary object code file.\n");
    exit(EXIT_FAILURE);
  }
<<<<<<< HEAD
  arm *state;

  // free memory before code termination
=======

  arm *state = malloc(sizeof(struct arm));
  init_arm(state, argv[1]);

>>>>>>> 3ae83d21e1d3c9d83ae4bb2cd92ba566686f12d8
  free(state->memory);
  free(state->registers);
  free(state);

  return EXIT_SUCCESS;
}
