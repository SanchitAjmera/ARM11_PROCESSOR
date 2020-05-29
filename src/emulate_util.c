#include "emulate_util.h"
#include "constants.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };
// condition suffixes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };

void check_ptr(const void *ptr, const char *error_msg) {
  if (ptr == NULL) {
    printf("Error: %s\n", error_msg);
    exit(EXIT_FAILURE);
  }
}

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
void init_arm(arm *state, const char *fname) {

  /* load binary file into memory */
  byte *memory = (byte *)calloc(MEM_BYTE_CAPACITY, sizeof(byte));
  check_ptr(memory, "Not enough memory.\n");

  FILE *bin_obj = fopen(fname, "rb");
  check_ptr(bin_obj, "File could not be opened\n");

  fseek(bin_obj, SEEK_SET, SEEK_END);
  long file_size = ftell(bin_obj);
  rewind(bin_obj);

  /* Asserts that fread read the whole file */
  assert(fread(memory, 1, file_size, bin_obj) == file_size);

  printf("Read %ld words into memory.\n", file_size / WORD_LEN);

  fclose(bin_obj);

  /* initialise registers */
  word *registers = (word *)calloc(REG_COUNT, sizeof(word));

  /* construct ARM state */
  state->memory = memory;
  state->registers = registers;
}

// TODO: consider const byte *start_addr
word get_word(byte *start_addr) {
  word w = 0;
  for (int i = 0; i < WORD_LEN; i++) {
    w += start_addr[i] << 8 * i;
  }
  return w;
}

void executeMultiply(arm *state, word instruction) {
  // Extraction of information from the instruction;
  int destination = (instruction & MULT_RDEST_MASK) >> MULT_RDEST_SHIFT;
  int regS = (instruction & MULT_REG_S_MASK) >> MULT_REG_S_SHIFT;
  int regM = instruction & MULT_REG_M_MASK;

  // Initial execution of instruction
  int result = state->registers[regM] * state->registers[regS];

  // Obtain value from Rn and add to result if Accumulate is set
  if (instruction & ACCUMULATE_FLAG) {
    int regN = (instruction & MULT_REG_N_MASK) >> MULT_REG_N_SHIFT;
    result += state->registers[regN];
  }
  // Update CPSR flags if S (bit 20 in instruction) is set
  if (UPDATE_CPSR(instruction)) {
    state->registers[CPSR] |= (result & CPSR_N_MASK);
    if (!result)
      state->registers[CPSR] |= CPSR_Z_MASK;
  }
  state->registers[destination] = result;
}

void executeBranch(arm *state, word instruction) {
  // Extraction of information
  int offset = instruction & BRANCH_OFFSET_MASK;
  int signBit = offset & BRANCH_SIGN_BIT;

  // Shift, sign extension and addition of offset onto current address
  state->registers[PC] +=
      (offset << CURRENT_INSTRUCTION_SHIFT) |
      (signBit ? NEGATIVE_SIGN_EXTEND : POSITIVE_SIGN_EXTEND);
}

bool checkCond(arm *state, word instruction) {
  // CPSR flag bits
  word cpsr = state->registers[CPSR];
  uint n = GET_CPSR_N(cpsr);
  uint z = GET_CPSR_Z(cpsr);
  uint v = GET_CPSR_V(cpsr);
  enum Cond cond = GET_CPSR_FLAGS(cpsr);
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
    // no other instruction
    // should never happen
    assert(false);
  }
}

void decode(arm *state, word instruction) {
  if (!checkCond(state, instruction)) {
    return;
  }

  // TODO: determine how to differentiate ...
  // ... `data processing` from `multiply`

  if (BITS_SET(instruction, DECODE_BRANCH_MASK, DECODE_BRANCH_EXPECTED)) {
    executeBranch(state, instruction);
  } else if (BITS_SET(instruction, DECODE_SDT_MASK, DECODE_SDT_EXPECTED)) {
    // executestdi(state, instruction)
  } else if (BITS_SET(instruction, DECODE_MULT_MASK, DECODE_MULT_EXPECTED)) {
    executeMultiply(state, instruction);
  } else if (BITS_SET(instruction, DECODE_DPI_MASK, DECODE_DPI_EXPECTED)) {
    // executedpi(state, instruction);
  }
}
