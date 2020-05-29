#include "emulate_util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "Please specify an ARM binary object code file.\n");
    exit(EXIT_FAILURE);
  }

  arm *state = malloc(sizeof(arm));
  init_arm(state, argv[1]);

  free(state->memory);
  free(state->registers);
  free(state);

  return EXIT_SUCCESS;
}
