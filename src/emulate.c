#include <stdlib.h>
#include <stdio.h>

#include "emulate_util.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Please specify an ARM binary object code file.\n");
		exit(EXIT_FAILURE);
	}

	struct arm* state = malloc(sizeof(struct arm));
	init_arm(state, argv[1]);

	free(state->memory);
	free(state->registers);
	free(state);

	return EXIT_SUCCESS;
}
