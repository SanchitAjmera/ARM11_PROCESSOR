#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "emulate_util.h"

void test_bool(bool cond, char* test_name) {
	printf("T: %s : %s\n", test_name, cond ? "OK":"FAIL");
}

/* Loads a binary file fname into state memory and returns strcmp() between memory
	 contents and the expected file dump fdump */
int test_file_load(struct arm* state, const char* fname, const char* fdump) {
	init_arm(state, fname);

	/* read state->memory contents into string */
	char mem_contents[strlen(fdump)]; 
	for (int i = 0; i < strlen(fdump)/2; i++) {
		sprintf(&mem_contents[2*i], "%02x", state->memory[i]);
	}
	printf("expected: %s, memory: %s\n", fdump, mem_contents);

	int non_zero_count = 0;
	for (int i = strlen(fdump); i < MEM_BYTE_CAPACITY; i++) {
		non_zero_count += state->memory[i]==0 ? 0 : 1;
	}
	test_bool(non_zero_count==0, "all other memory locations 0");

	free(state->memory);
	free(state->registers);

	return strcmp(mem_contents, fdump);
}

/* Tests whether binary object files are loaded into memory 1) correctly,
	 2) in their entirety, and 3) in the correct byte order. */
void test_init_arm() {
	struct arm* state = malloc(sizeof(struct arm));

	/* hex dump of binary object files used in this test */
	char* add01_dump = "0110a0e3022081e2";
	int add01_cmp = test_file_load(state, "../../arm11_testsuite/test_cases/add01", add01_dump);
	test_bool(add01_cmp==0, "memory contents match add01 file");

	char* add02_dump = "0110a0e30220a0e3023081e0";
	int add02_cmp = test_file_load(state, "../../arm11_testsuite/test_cases/add02", add02_dump);
	test_bool(add02_cmp==0, "memory contents match add02 file");

	char* str03_dump = "0220a0e30100a0e36310a0e3031080e5012042e2000052e3081080e5fbffff1a";
	int str03_cmp = test_file_load(state, "../../arm11_testsuite/test_cases/str03", str03_dump);
	test_bool(str03_cmp==0, "memory contents match str03 file");

	char* gpio_2_dump = "0120a0e38224a0e128009fe5002080e50110a0e38111a0e1281080e50a20a0e3281080e5012042e2000052e31c1080e5faffff1a0000000000002020";
	int gpio_2_cmp = test_file_load(state, "../../arm11_testsuite/test_cases/gpio_2", gpio_2_dump);
	test_bool(gpio_2_cmp==0, "memory contents match gpio_2 file");

	free(state);
}

int main(void) {	
	test_init_arm();
	return 0;
}
