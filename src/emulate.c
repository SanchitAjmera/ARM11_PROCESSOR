#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

typedef uint32_t word;

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };

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

// From Alex with a minor update to refactor the mask so the instruction is an
// input
bool checkCond(arm state, word instruction) {
  unsigned int cond = instruction & 0xF0000000;
  // CPSR flag bits
  unsigned int n = state.registers[CPSR] & 8;
  unsigned int z = state.registers[CPSR] & 4;
  unsigned int c = state.registers[CPSR] & 2;
  unsigned int v = state.registers[CPSR] & 1;
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

void multiply(arm state, word instruction) {
  if (!checkCond(state, instruction))
    return;
  int result;

  // Extraction of information from the instruction;
  int destination = (instruction & 0x000F0000) >> 16;
  int regS = (instruction & 0x00000F00) >> 8;
  int regM = instruction & 0x0000000F;
  result = state.registers[regM] * state.registers[regS];

  // Obtain value from Rn and add to result if Accumulate is set
  // need to confirm arguments of BITS_SET with Alex
  if (BITS_SET(instruction, 0x00200000, 0x00200000)) {
    int regN = (instruction & 0x0000F000) >> 12;
    result += state.registers[regN];
  }

  state.registers[destination] = result;

  /*TO DO:
    - Set CPSR values according to if S bit is set
    - Possibly figure out way of reducing magic number masks
  */
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

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Please specify an ARM binary object code file.\n");
    exit(EXIT_FAILURE);
  }
  arm *state;

  // free memory before code termination
  free(state->memory);
  free(state->registers);
  free(state);
  return EXIT_SUCCESS;
}
