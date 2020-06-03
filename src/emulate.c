#include "constants.h"
#include "emulate_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArmState(arm_t *state) {
  printf("Registers:\n");
  char registerName[5];
  for (int i = 0; i < NUM_REGISTERS; i++) {
    if (i == 13 || i == 14) {
      // Not used in this exercise
      continue;
    }

    if (i == 15) {
      strcpy(registerName, "PC");
    } else if (i == 16) {
      strcpy(registerName, "CPSR");
    } else {
      sprintf(registerName, "$%i", i);
    }

    printf("%-4s: %10i (0x%08x)\n", registerName, state->registers[i],
           state->registers[i]);
  }

  printf("Non-zero memory:\n");
  for (int i = 0; i < MEMORY_CAPACITY; i += 4) {
    word memoryVal = getWord(state->memory + i, IS_BIG_ENDIAN);
    if (memoryVal != 0)
      printf("0x%08x: 0x%08x\n", i, memoryVal);
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "Please specify an ARM binary object code file.\n");
    exit(EXIT_FAILURE);
  }

  arm_t *state = malloc(sizeof(arm_t));
  initArm(state, argv[1]);
  decoded_t *decoded = malloc(sizeof(decoded_t));

  // PIPELINE
  while ((state->decoded.isSet && state->decoded.instruction) ||
         !state->decoded.isSet) {
    execute(state, decoded);
    decode(state, decoded);
    fetch(state);
  }

  printArmState(state);

  // free memory before code termination
  free(state->memory);
  free(state->registers);
  free(state);
  free(decoded);
  return EXIT_SUCCESS;
}
