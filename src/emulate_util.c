#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "emulate_util.h"

void check_ptr(const void* ptr, const char* error_msg) {
	if (ptr == NULL) {
		printf("Error: %s\n", error_msg);
		exit(EXIT_FAILURE);
	}
}

/* Takes in the ARM binary file's name and returns an ARM state pointer with memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
void init_arm(struct arm* state, const char* fname) {
	
	/* load binary file into memory */
	Byte* memory = (Byte*) calloc(MEM_BYTE_CAPACITY, sizeof(Byte));
	check_ptr(memory, "Not enough memory.\n");

	FILE* bin_obj = fopen(fname, "rb");
	check_ptr(bin_obj, "File could not be opened\n");

	fseek(bin_obj, 0, SEEK_END);
	long file_size = ftell(bin_obj);
	rewind(bin_obj);

	assert(file_size < MEM_BYTE_CAPACITY);
	assert(file_size % WORD_LEN == 0);
	assert(fread(memory, 1, file_size, bin_obj)==file_size);

	printf("Read %ld words into memory.\n", file_size/WORD_LEN);

	fclose(bin_obj);

	/* initialise registers */
	Word* registers = (Word*) calloc(REG_COUNT, sizeof(Word));

	/* construct ARM state */
	state->memory = memory;
	state->registers = registers;
}
