
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common/constants.h"
#include "emulator/emulate_util.h"

void testBool(bool cond, char *test_name) {
  printf("T: %s : %s\n", test_name, cond ? "OK" : "FAIL");
}

/* Loads a binary file fname into state memory and returns strcmp() between
   memory contents and the expected file dump fdump */
int testFileLoad(arm_t *state, const char *fname, const char *fdump) {
  initArm(state, fname);

  /* read state->memory contents into string */
  char memoryContents[strlen(fdump)];
  for (int i = 0; i < strlen(fdump) / 2; i++) {
    sprintf(&memoryContents[2 * i], "%02x", state->memory[i]);
  }
  printf("expected: %s, memory: %s\n", fdump, memoryContents);

  int nonZeroCount = 0;
  for (int i = strlen(fdump); i < MEMORY_CAPACITY; i++) {
    nonZeroCount += state->memory[i] == 0 ? 0 : 1;
  }
  testBool(nonZeroCount == 0, "all other memory locations 0");

  free(state->memory);
  free(state->registers);

  return strcmp(memoryContents, fdump);
}

/* Tests whether binary object files are loaded into memory
  1) correctly,
  2) in their entirety
  3) in the correct byte order. */
void testInitArm() {
  arm_t *state = malloc(sizeof(arm_t));

  /* hex dump of binary object files used in this test */
  char *add01Dump = "0110a0e3022081e2";
  int add01Cmp =
      testFileLoad(state, "../../arm11_testsuite/test_cases/add01", add01Dump);
  testBool(add01Cmp == 0, "memory contents match add01 file");

  char *add02Dump = "0110a0e30220a0e3023081e0";
  int add02Cmp =
      testFileLoad(state, "../../arm11_testsuite/test_cases/add02", add02Dump);
  testBool(add02Cmp == 0, "memory contents match add02 file");

  char *str03Dump =
      "0220a0e30100a0e36310a0e3031080e5012042e2000052e3081080e5fbffff1a";
  int str03Cmp =
      testFileLoad(state, "../../arm11_testsuite/test_cases/str03", str03Dump);
  testBool(str03Cmp == 0, "memory contents match str03 file");

  char *gpio2Dump =
      "0120a0e38224a0e128009fe5002080e50110a0e38111a0e1281080e50a20a0e3281080e5"
      "012042e2000052e31c1080e5faffff1a0000000000002020";
  int gpio2Cmp =
      testFileLoad(state, "../../arm11_testsuite/test_cases/gpio_2", gpio2Dump);
  testBool(gpio2Cmp == 0, "memory contents match gpio_2 file");

  free(state);
}

void testCPSR() {
  word op1, op2, result, expected, carryOut;

  // Test addition
  op1 = 0xAFFFFFFF;
  op2 = 0x6FFFFFFF;
  result = op1 + op2;
  expected = 0x1FFFFFFE;
  carryOut = op1 <= UINT32_MAX - op2 ? 0 : 1;
  testBool(carryOut == 1,
           "0xAFFFFFFF+0x6FFFFFFF (unsigned overflow), carry = 1");
  testBool(result == expected, "0xAFFFFFFF+0x6FFFFFFF, result = 0x1FFFFFFE");

  op1 = 0x000003E8;
  op2 = 0x000AE200;
  result = op1 + op2;
  carryOut = op1 <= UINT32_MAX - op2 ? 0 : 1;
  testBool(carryOut == 0, "0x000003E8+0x000AE200, carry = 0");

  // Test subtraction
  op1 = 0x0FFFFFFF;
  op2 = 0xFFFFFFFF;
  result = op1 - op2;
  carryOut = op1 < op2 ? 0 : 1;
  testBool(carryOut == 0, "0x0FFFFFFF-0xFFFFFFFF, carry = 0");

  op1 = 20;
  op2 = 50;
  result = op1 - op2;
  carryOut = op1 < op2 ? 0 : 1;
  testBool(carryOut == 0, "20 - 50, carry = 0");

  op1 = 50;
  op2 = 20;
  result = op1 - op2;
  carryOut = op1 < op2 ? 0 : 1;
  testBool(carryOut == 1, "50 - 20, carry = 1");

  // TODO: add testing for the other CPSR flags? N Z C V
}

int main(void) {
  printf("----Testing initArm----\n");
  testInitArm();

  printf("----Testing CPSR flags----\n");
  testCPSR();
  return 0;
}
