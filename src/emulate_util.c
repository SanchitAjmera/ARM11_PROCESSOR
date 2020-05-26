#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "emulate_util.h"

void check_ptr(const void *ptr, const char *error_msg) {
  if (ptr == NULL) {
    printf("Error: %s\n", error_msg);
    exit(EXIT_FAILURE);
  }
}

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
void init_arm(struct arm *state, const char *fname) {

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
