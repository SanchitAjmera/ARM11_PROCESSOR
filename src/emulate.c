#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define MEM_LIMIT 65536
#define WORD_LEN 4
#define REG_COUNT 17

typedef uint32_t word;

struct arm {
	word*	memory;

	/* 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR */
	word*	registers;
};

void check_ptr(const void* ptr, char* error_msg) {
	if (ptr == NULL) {
		printf("Error: %s\n", error_msg);
		exit(EXIT_FAILURE);
	}
}

/* Takes in the ARM binary file's name and returns an ARM state pointer with memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
struct arm* init_arm(char* fname) {
	
	/* load binary file into memory */
	word* memory = (word*) calloc(MEM_LIMIT/sizeof(word), sizeof(word));
	check_ptr(memory, "Not enough memory.\n");

	FILE* bin_obj = fopen(fname, "rb");
	check_ptr(bin_obj, "File could not be opened\n");

	fseek(bin_obj, 0, SEEK_END);
	long file_size = ftell(bin_obj);
	rewind(bin_obj);

	assert(file_size < MEM_LIMIT);
	assert(file_size % WORD_LEN == 0);
	fread(memory, sizeof(word), file_size/WORD_LEN, bin_obj);

	printf("Read %ld words into memory.\n", file_size/WORD_LEN);

	fclose(bin_obj);

	/* initialise registers */
	word* registers = (word*) calloc(REG_COUNT, sizeof(word));

	/* construct ARM state */
	struct arm* arm_state = malloc(sizeof(struct arm));
	arm_state->memory = memory;
	arm_state->registers = registers;

	return arm_state;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Please specify an ARM binary object code file.\n");
		exit(EXIT_FAILURE);
	}

	struct arm* arm_state = init_arm(argv[1]);

	free(arm_state->memory);
	free(arm_state->registers);
	free(arm_state);

	return EXIT_SUCCESS;
}
