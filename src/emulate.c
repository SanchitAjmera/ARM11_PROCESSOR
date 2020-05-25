#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits)((value & mask) == bits)

typedef uint32_t word;

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register {PC = 15, CPSR = 16};
enum Opcode {AND = 0, EOR = 1, SUB = 2, RSB = 3, ADD = 4, TST = 8, TEQ = 9, CMP = 10, ORR = 12, MOV = 13};

typedef struct {
  // ARM machine memory
  uint8_t * memory;
  // 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR
  word * registers;
} arm;

void ptrValidate(const void * pointer, char* error) {
  if (pointer == NULL) {
    printf("Error: %s\n", error);
    exit(EXIT_FAILURE);
  }
}

void dpi(word instruction) {
  const word mask = 0x01E00000;
  enum Opcode opcode = instruction & mask;
  switch (opcode) {
    case AND:
      // Rn AND operand2
      break;
    case EOR:
      // Rn EOR operand2
      break;
    case SUB:
      // Rn - operand2
      break;
    case RSB:
      // operand2 - Rn
      break;
    case ADD:
      // Rn + operand2
      break;
    case TST:
      // as and, but result not written
      break;
    case TEQ:
      // as eor, but result is not written
      break;
    case CMP:
      // as sub, but result is not written
      break;
    case ORR:
      // Rn OR operand2
      break;
    case MOV:
      // operand2 (Rn is ignored)
      break;
  }
}

void instructionSelect(word instruction) {
  const word dpMask = 0x0C000000;
  const word dp = 0x00000000;
  const word multMask = 0x0FC000F0;
  const word mult = 0x0000090;
  const word sdtMask = 0x0C600000;
  const word sdt = 0x04000000;
  const word branchMask = 0x0F000000;
  const word branch = 0x0A000000;

  // TODO: determine how to differentiate ...
  // ... `data processing` from `multiply`

  if (BITS_SET(instruction, branchMask, branch)) {
    // function for branch instructions
  } else if (BITS_SET(instruction, sdtMask, sdt)) {
    // function for single data tranfser instructions
  } else if (BITS_SET(instruction, multMask, mult)) {
    // function for multiply instructions
  } else if (BITS_SET(instruction, dpMask, dp)) {
    dpi(instruction);
  }
}

int main(int argc, char ** argv) {
  if (argc == 1) {
		printf("Please specify an ARM binary object code file.\n");
		exit(EXIT_FAILURE);
	}

	arm * state;

  // free memory before code termination
	free(state -> memory);
	free(state -> registers);
	free(state);
  return EXIT_SUCCESS;
}
