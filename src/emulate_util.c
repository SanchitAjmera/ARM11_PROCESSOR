#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "emulate_util.h"

#define BITS_SET(value, mask, bits) ((value & mask) == bits)

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };

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

  fseek(bin_obj, 0, SEEK_END);
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

word get_word(byte *start_addr) {
  word w = 0;
  for (int i = 0; i < WORD_LEN; i++) {
    w += start_addr[i] << 8 * i;
  }
  return w;
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
