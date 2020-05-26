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
  uint n = (state.registers[CPSR] & 0x80000000) >> 31;
  uint z = (state.registers[CPSR] & 0x40000000) >> 30;
  uint c = (state.registers[CPSR] & 0x20000000) >> 29;
  uint v = (state.registers[CPSR] & 0x10000000) >> 28;
  enum Cond cond = instruction >> 28;
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

word rotateRight(word value, uint rotateBy) {
  // word temp = 0x00000000;
  uint start;
  uint rotateBits = value & rotateBy;
  return 0;
}

word shiftI(word value, uint shiftNum, enum Shift shiftType) {
  // TODO: set the CPSR flags (C carry out bit)
  switch (shiftType) {
  case LSL:
    return value << shiftNum;
  case LSR:
    return value >> shiftNum;
  case ASR:
    // TODO: check if this is an arithmetic shift
    return (signed int)value > shiftNum;
  case ROR:
    break;
  }
}

uint shiftByConstant(uint shiftPart) {
  // integer specified by bits 7-4
  return shiftPart >> 4;
}

uint shiftByRegister(arm state, uint shiftPart) {
  // Rs (register) can be any general purpose register except the PC
  uint rs = shiftPart >> 4;
  // bottom byte of value in Rs specifies the amount to be shifted
  return state.registers[rs] & 0x0000000F;
}

word opRegister(arm state, uint op2) {
  // value to be shifted
  word value = state.registers[op2 & 0x00F];
  // bits indicating the shift instruction
  uint shiftPart = op2 >> 4;
  // shift type instruction
  enum Shift shiftType = (shiftPart & 0x06) >> 1;
  // bit to determine what to shift by
  bool shiftByConst = shiftPart & 0x01;
  // number to shift by
  uint shiftNum = shiftByConst ? shiftByConstant(shiftPart)
                               : shiftByRegister(state, shiftPart);
  return shiftI(value, shiftNum, shiftType);
}

word opImmediate(arm state, uint op2) {
  // 8-bit immediate value zero-extended to 32 bits
  word imm = (op2 & 0x0FF) << 24;
  // number of rotations
  uint rotateBy = (op2 & 0xF00) >> 8;
  return rotateRight(imm, rotateBy);
}

void dpi(arm state, word instruction) {
  if (!checkCond(state, instruction)) {
    return;
  }
  // If i is set, op2 is an immediate const, otherwise it's a shifted register
  const uint i = (instruction & 0x02000000) >> 25;
  // instruction to execute
  enum Opcode opcode = (instruction & 0x01E00000) >> 20;
  // if s is set then the CPSR flags should be updated
  const uint s = (instruction & 0x00100000) >> 20;
  // op1 is always the contents of register Rn
  const uint rn = (instruction & 0x000F0000) >> 16;
  // destination register
  const uint rd = (instruction & 0x0000F000) >> 12;
  // second operand
  word op2 = instruction & 0x00000FFF;

  // TODO: CPSR flags

  word op1 = state.registers[rn];
  op2 = i ? opImmediate(state, op2) : opRegister(state, op2);
  // execution of instruction
  switch (opcode) {
  case AND:
    state.registers[rd] = op1 & op2;
    break;
  case EOR:
    state.registers[rd] = op1 ^ op2;
    break;
  case SUB:
    state.registers[rd] = op1 - op2;
    break;
  case RSB:
    state.registers[rd] = op2 - op1;
    break;
  case ADD:
    state.registers[rd] = op1 + op2;
    break;
  case TST:
    op1 &op2;
    break;
  case TEQ:
    op1 ^ op2;
    break;
  case CMP:
    op1 - op2;
    break;
  case ORR:
    state.registers[rd] = op1 | op2;
    break;
  case MOV:
    state.registers[rd] = op2;
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
