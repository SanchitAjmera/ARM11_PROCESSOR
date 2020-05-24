#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define MEM_LIMIT 65536
#define WORD_LEN 4

void check_ptr(const void* ptr, char* error_msg) {
	if (ptr == NULL) {
		printf("Error: %s\n", error_msg);
		exit(EXIT_FAILURE);
	}
}

uint32_t* read_arm_to_mem(char* fname) {

	uint32_t* memory = (uint32_t*) calloc(MEM_LIMIT, sizeof(uint32_t));
	check_ptr(memory, "Not enough memory.\n");

	FILE* bin_obj = fopen(fname, "rb");
	check_ptr(bin_obj, "File could not be opened\n");

	fseek(bin_obj, 0, SEEK_END);
	long file_size = ftell(bin_obj);
	rewind(bin_obj);

	assert(file_size < MEM_LIMIT);
	assert(file_size % WORD_LEN == 0);
	fread(memory, sizeof(uint32_t), file_size/WORD_LEN, bin_obj);

	printf("Read %ld words into memory.\n", file_size/WORD_LEN);

	fclose(bin_obj);
	return memory;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Please specify an ARM binary object code file.\n");
		exit(EXIT_FAILURE);
	}

	uint32_t* memory = read_arm_to_mem(argv[1]);



	free(memory);

	return EXIT_SUCCESS;
}
