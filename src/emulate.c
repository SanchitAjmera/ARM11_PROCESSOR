#include "common/constants.h"
#include "emulator/decode/emulate_decode.h"
#include "emulator/emulate_constants.h"
#include "emulator/emulate_util.h"
#include "emulator/execute/emulate_execute.h"
#include "emulator/fetch/emulate_fetch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArmState(arm_t *state) {
  printf("Registers:\n");
  char registerName[MAX_REGISTER_NAME_LENGTH];
  for (int i = 0; i < NUM_REGISTERS; i++) {
    if (i == 13 || i == 14) {
      // Not used in this exercise
      continue;
    }

    if (i == PC) {
      strcpy(registerName, "PC");
    } else if (i == CPSR) {
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
