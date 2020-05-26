#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

typedef uint32_t word;
typedef unsigned int uint;

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };
// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// condition suffixes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };
// shift types
enum Shift { LSL, LSR, ASR, ROR };

typedef struct {
  // ARM machine memory
  uint8_t *memory;
  // 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR
  word *registers;
} arm;

void ptrValidate(const void *pointer, char *error) {
  if (pointer == NULL) {
    printf("Error: %s\n", error);
    exit(EXIT_FAILURE);
  }
}

bool checkCond(arm state, word instruction) {
  // CPSR flag bits
  uint n = state.registers[CPSR] & 0x80000000;
  uint z = state.registers[CPSR] & 0x40000000;
  uint c = state.registers[CPSR] & 0x20000000;
  uint v = state.registers[CPSR] & 0x10000000;
  uint cond = instruction >> 28;
  // conditions for instruction
  switch (cond) {
  case EQ:
    return z;
  case NE:
    return !z;
  case GE:
    return n == v;
  case LT:
    return n != v;
  case GT:
    return !z && (n == v);
  case LE:
    return z || (n != v);
  case AL:
    return true;
  default:
    return false;
  }
}

uint shiftConstant(uint shiftPart) {
  // integer specified by bits 7-4
  return shiftPart >> 4;
}

uint shiftRegister(arm state, uint rs) {
  // pre: Rs (register) can be any general purpose register except the PC
  // bottom byte of Rs specifies the amount to be shifted
  return state.registers[rs] & 0x0000000F;
}

uint shiftI(word value, uint shiftBy, uint shiftType) {
  // TODO: set the CPSR flags (C carry out bit)
  switch (shiftType) {
  case LSL:
    return value << shiftBy;
  case LSR:
    return value >> shiftBy;
  case ASR:
    // TODO: check if this is an arithmetic shift
    return (signed int)value > shiftBy;
  case ROR:
    break;
  }
}

int opRegister(arm state, uint op2) {
  // value to be shifted
  word value = state.registers[op2 & 0x00F];
  // bits indicating the shift instruction
  uint shiftPart = op2 >> 4;
  // shift type instruction
  uint shiftType = (shiftPart & 0x06) >> 1;
  // number to shift by
  uint shiftBy;
  if (shiftPart & 0x01) {
    // shift by a constant
    shiftBy = shiftConstant(shiftPart);
  } else {
    // shift specified by a register (optional)
    shiftBy = shiftRegister(state, shiftPart >> 4);
  }
  return shiftI(value, shiftBy, shiftType);
}

int opImmediate(arm state, uint op2) {
  // TODO: code me
  return 0;
}

void dpi(arm state, word instruction) {
  if (!checkCond(state, instruction)) {
    return;
  }
  // parts of the instruction
  uint i = (instruction & 0x02000000) >> 25;
  uint s = (instruction & 0x00100000) >> 20;
  uint rn = (instruction & 0x000F0000) >> 16;
  uint rd = (instruction & 0x0000F000) >> 12;
  uint op2 = instruction & 0x00000FFF;

  // TODO:
  // If the S bit is 0, the CPSR register is unaffected
  // If the S bit is set then the CPSR flags should be set as follows:
  // The V bit will be unaffected.
  // The C bit in logical operations will be set to the carry out from any shift
  // operation In arithmetic operations the C bit will be set to the carry out
  // of the bit 31 of the ALU C is set to 1 if the addition produced a carry For
  // subtraction (including comparison), the bit C is set to 0 if the
  // subtraction produced a borrow
  // The Z bit will be set only if the result is all zeros.
  // The N bit will be set to the logical value of bit 31 of the result.

  // TODO: operand2 is an immediate value

  // op2
  word value;
  if (i) {
    // op2 is an immediate value
    value = opImmediate(state, op2);
  } else {
    // op2 is a register value
    value = opRegister(state, op2);
  }

  // execution of instruction
  word src = state.registers[rn];
  word dest = state.registers[rd];
  enum Opcode opcode = instruction & 0x01E00000;
  switch (opcode) {
  case AND:
    // Rn AND operand2
    state.registers[rd] = state.registers[rn] & value;
    break;
  case EOR:
    // Rn EOR operand2
    state.registers[rd] = state.registers[rn] ^ value;
    break;
  case SUB:
    // Rn - operand2
    state.registers[rd] = state.registers[rn] - value;
    break;
  case RSB:
    // operand2 - Rn
    state.registers[rd] = value - state.registers[rn];
    break;
  case ADD:
    // Rn + operand2
    state.registers[rd] = state.registers[rn] + value;
    break;
  case TST:
    // as and, but result not written
    state.registers[rn] & value;
    break;
  case TEQ:
    // as eor, but result is not written
    state.registers[rd] = state.registers[rn] ^ value;
    break;
  case CMP:
    // as sub, but result is not written
    state.registers[rd] = state.registers[rn] - value;
    break;
  case ORR:
    // Rn OR operand2
    state.registers[rd] = state.registers[rn] | value;
    break;
  case MOV:
    // operand2 (Rn is ignored)
    state.registers[rd] = value;
    break;
  }
}

void decode(arm state, word instruction) {
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
    dpi(state, instruction);
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Please specify an ARM binary object code file.\n");
    exit(EXIT_FAILURE);
  }
  arm *state;

  // free memory before code termination
  free(state->memory);
  free(state->registers);
  free(state);
  return EXIT_SUCCESS;
}
