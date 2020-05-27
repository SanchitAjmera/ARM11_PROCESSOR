#ifndef EMULATE_UTIL_H
#define EMULATE_UTIL_H

#include <stdint.h>
#include <stdlib.h>

typedef uint32_t word;
typedef uint8_t byte;
typedef struct arm arm;

static const int MEM_BYTE_CAPACITY = 65536;
static const int WORD_LEN = 4;
static const int REG_COUNT = 17;

struct arm {
  byte *memory;

  /* 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR */
  word *registers;
};

extern void check_ptr(const void *ptr, const char *error_msg);

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
extern void init_arm(arm *state, const char *fname);

extern void decode(arm state, word instruction);

#endif