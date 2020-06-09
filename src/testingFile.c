

#include "constants.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t word;
typedef unsigned int uint;

typedef struct instruction {
  char *opcode;  // String representing the instruction INCLUDING cond suffix
  char **fields; // Array of string fields.
  uint field_count;
  /*
  We can add a union of structs that represent dpi, sdri, branch, mult later
  if this makes it easier to code
  */
} instruction;

word rem(char *string) {
  char *c = "[";
  if (&string[0] == &c[0]) {
    return atoi(string + 2);
  }
  return atoi(string++);
}

void printBits(uint32_t x) {
  uint32_t mask = 1 << 31;

  for (int i = 0; i < 32; i++) {
    if ((x & mask) == 0) {
      printf("0");
    } else {
      printf("1");
    }
    x = x << 1;
  }
  printf("\n");
}

word assembleSDTI(instruction *input) {
  char *ldr = "ldr";
  // Load bit
  word l;
  (strcmp(input->opcode, ldr) == 0) ? (l = 0 << 31) : (l = 1 << 31);
  // PRE/POST-INDEXING bits
  word p;
  // base register Rn
  word Rn;
  // offsets
  word offset;
  // source/ dest register Rd
  word Rd = rem(input->fields[0]) << SDTI_RD_SHIFT;
  if (input->field_count == 3) {
    p = 1 << SDTI_P_SHIFT;
    // base register Rn
    Rn = rem(input->fields[1]) << SDTI_RN_SHIFT;
    // offset
    offset = rem(input->fields[2]);
  }
  // immediate offsets
  word i = 1 << SDTI_I_SHIFT;
  // up bits
  word u = 1 << SDTI_U_SHIFT;

  word always = 1 << 31;
  word set = 1 << 26;
  word concat = always | set | i | p | u | l | Rn | Rd | offset;
  return concat;
}

/*
int main(void) {
  uint32_t cond = 1 << 31;
  uint32_t randoms = 1 << 26;
  uint32_t immediateOffset = 1 << 25;
  uint32_t prepostindex = 1 << 24;
  uint32_t up = 1 << 23;
  uint32_t l = 1 << 20;
  uint32_t rn1 = 3;
  uint32_t rn2 = 15;
  uint32_t rn = rn1 << 16;
  uint32_t rd = rn2 << 12;
  uint32_t offset = 0 << 11;

  uint32_t concat = cond | randoms | immediateOffset | prepostindex | up | l |
                    rn | rd | offset;

  // printf("%" PRIu32 "\n", )
  printBits(concat);
  return 1;
}
*/

int main(void) {
  // printf("yo\n");
  // instruction *input = malloc(sizeof(instruction));
  // input->opcode = "ldr";
  // input->fields[0] = "[r3]";
  // printf("%s\n", input->fields[0]);
  // input->fields[1] = "0x10";
  // input->field_count = 3;
  // word output = assembleSDTI(input);
  // printf("%" PRIu32 "\n", output);
  // free(input);

  char *address = "[r3,#expression]";
  // printBits(rem(address));
  // printBits(rem("2"));
  char *delim = ",";
  char *ptr = strtok(address, delim);
  printf("%s\n", ptr);

  return 1;
}
