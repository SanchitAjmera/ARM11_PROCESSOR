#include "constants.h"
#include "emulate_util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_arm_state(arm *state) {
  printf("Registers:\n");
  char reg_name[5];
  for (int i = 0; i < NO_REGISTERS; i++) {
    if (i == 13 || i == 14)
      continue; // Not used in this exercise

    if (i == 15) {
      strcpy(reg_name, "PC");
    } else if (i == 16) {
      strcpy(reg_name, "CPSR");
    } else {
      sprintf(reg_name, "%%%i", i);
    }

    printf("%-4s:%10i (0x%08x)\n", reg_name, state->registers[i],
           state->registers[i]);
  }

  printf("Non-zero memory:\n");
  for (int i = 0; i < MEMORY_CAPACITY; i += 4) {
    if (state->memory[i] != 0)
      printf("0x%08x: 0x%08x\n", i, get_word(state->memory + i));
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "Please specify an ARM binary object code file.\n");
    exit(EXIT_FAILURE);
  }

  // free memory before code termination
  arm *state = malloc(sizeof(arm));
  init_arm(state, argv[1]);

  word instruction;
  do {
    instruction = fetch(state);
    printf("%08x\n", instruction);

    // TODO: decode(state, instruction)
    // TODO: execute(state) ??
  } while (instruction != 0);

  print_arm_state(state);

  free(state->memory);
  free(state->registers);
  free(state);

  return EXIT_SUCCESS;
}
