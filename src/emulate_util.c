#include "emulate_util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS_SET(value, mask, bits) ((value & mask) == bits)

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };
// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// condition suffixes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };
// shift types
enum Shift { LSL, LSR, ASR, ROR };

void check_ptr(const void *ptr, const char *error_msg) {
  if (ptr == NULL) {
    printf("Error: %s\n", error_msg);
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

// dpi ----------------------------------------------------------------------

word rotateRight(word value, uint rotateNum) {
  // the msbs shifted right
  uint first = value & (32 - rotateNum) >> rotateNum;
  // the lsbs to rotate
  uint second = (value & rotateNum) << (32 - rotateNum);
  return first | second;
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

word shiftI(arm state, word value, uint shiftPart) {
  // TODO: set the CPSR flags (C carry out bit)
  // bit to determine what to shift by
  bool shiftByConst = shiftPart & 0x01;
  // number to shift by
  uint shiftNum = shiftByConst ? shiftByConstant(shiftPart)
                               : shiftByRegister(state, shiftPart);
  enum Shift shiftType = (shiftPart & 0x06) >> 1;
  switch (shiftType) {
  case LSL:
    return value << shiftNum;
  case LSR:
    return value >> shiftNum;
  case ASR:
    // TODO: CODE ME
    return 0;
  case ROR:
    return rotateRight(value, shiftNum);
  }
}

word opRegister(arm state, uint op2) {
  // register that holds the value to be shifted
  uint rm = op2 & 0x00F;
  // value to be shifted
  word value = state.registers[rm];
  // bits indicating the shift instruction
  uint shiftPart = op2 >> 4;
  return shiftI(state, value, shiftPart);
}

word opImmediate(arm state, uint op2) {
  // 8-bit immediate value zero-extended to 32 bits
  word imm = (op2 & 0x0FF) << 24;
  // number to rotate by
  uint rotateNum = (op2 & 0xF00) >> 8;
  return rotateRight(imm, rotateNum);
}

void setCPSR(arm state, word result) {
  word new = 0x00000000;
  // set to the logical value of bit 31 of the result
  word n = result & 0x80000000;
  // set only if the result is all zeros
  word z;
  if (!result) {
    z = 1 << 29;
  } else {
    z = 0;
  }
  // unaffected
  uint v = state.registers[CPSR] & 0x10000000;
  // updated flag bits
  new = n | z | c | v;
  state.registers[CPSR] = new;
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

  word op1 = state.registers[rn];
  op2 = i ? opImmediate(state, op2) : opRegister(state, op2);
  // execution of instruction
  word result;
  switch (opcode) {
  case TST:
    result = op1 & op2;
  case AND:
    state.registers[rd] = result;
    break;
  case TEQ:
    result = op1 ^ op2;
  case EOR:
    state.registers[rd] = result;
    break;
  case CMP:
    result = op1 - op2;
  case SUB:
    state.registers[rd] = result;
    break;
  case RSB:
    result = op2 - op1;
    state.registers[rd] = result;
    break;
  case ADD:
    result = op1 + op2;
    state.registers[rd] = result;
    break;
  case ORR:
    result = op1 | op2;
    state.registers[rd] = result;
    break;
  case MOV:
    result = op2;
    state.registers[rd] = result;
    break;
  }

  if (s) {
    setCPSR(state, result);
  }
}

// end of dpi ---------------------------------------------------------------

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
void init_arm(arm *state, const char *fname) {

  /* load binary file into memory */
  byte *memory = (byte *)calloc(MEM_BYTE_CAPACITY, sizeof(byte));
  check_ptr(memory, "Not enough memory.\n");

  FILE *bin_obj = fopen(fname, "rb");
  check_ptr(bin_obj, "File could not be opened\n");

  fseek(bin_obj, 0, SEEK_END);
  long file_size = ftell(bin_obj);
  rewind(bin_obj);

  /* Asserts that fread read the whole file */
  assert(fread(memory, 1, file_size, bin_obj) == file_size);

  printf("Read %ld words into memory.\n", file_size / WORD_LEN);

  fclose(bin_obj);

  /* initialise registers */
  word *registers = (word *)calloc(REG_COUNT, sizeof(word));

  /* construct ARM state */
  state->memory = memory;
  state->registers = registers;
}

word get_word(byte *start_addr) {
  word w = 0;
  for (int i = 0; i < WORD_LEN; i++) {
    w += start_addr[i] << 8 * i;
  }
  return w;
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
