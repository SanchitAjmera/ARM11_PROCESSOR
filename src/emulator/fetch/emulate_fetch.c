#include "../../common/constants.h"
#include "../../common/util.h"
#include "../emulate_constants.h"
#include "../emulate_util.h"
#include "../execute/emulate_execute.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEMORY_CAPACITY bytes */
void initArm(arm_t *state, const char *fname) {
  // load binary file into memory
  byte *memory = (byte *)calloc(MEMORY_CAPACITY, sizeof(*memory));
  validatePtr(memory, MEM_ASSIGN);
  // file handling
  FILE *binFile = fopen(fname, "rb");
  validatePtr(binFile, FILE_ERR);
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

word getWord(byte *startAddress, bool isBigEndian) {
  word w = 0;
  for (int i = 0; i < WORD_SIZE_BYTES; i++) {
    uint byte_index = isBigEndian ? (WORD_SIZE_BYTES - 1 - i) : i;
    w += startAddress[i] << BYTE * byte_index;
  }
  return w;
}

void fetch(arm_t *state) {
  word memoryOffset = state->registers[PC];
  state->registers[PC] += WORD_SIZE_BYTES;
  state->fetched = getWord(state->memory + memoryOffset, NOT_BIG_ENDIAN);
}
