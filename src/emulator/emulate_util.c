#include "emulate_util.h"
#include "../common/constants.h"
#include "../common/util.h"
#include "emulate_constants.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEMORY_CAPACITY bytes */
void initArm(arm_t *state, const char *fname) {
  // load binary file into memory
  byte *memory = (byte *)calloc(MEMORY_CAPACITY, sizeof(byte));
  errorExit(validatePtr(memory, "Not enough memory.\n"));
  // file handling
  FILE *binFile = fopen(fname, "rb");
  errorExit(validatePtr(binFile, "File could not be opened\n"));
  fseek(binFile, SEEK_SET, SEEK_END);
  long fileSize = ftell(binFile);
  rewind(binFile);
  // Asserts that fread read the whole file
  assert(fread(memory, 1, fileSize, binFile) == fileSize);
  fclose(binFile);
  // initialise registers
  word *registers = (word *)calloc(NUM_REGISTERS, sizeof(word));

  // construct ARM state
  state->memory = memory;
  state->registers = registers;
  flushPipeline(state);
}
