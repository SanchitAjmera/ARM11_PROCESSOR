#include "emulate_util.h"
#include "constants.h"
#include "emulate_util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: change name
typedef struct {
  word result;
  word carryOut;
} tuple_t;

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
    // no other instruction
    // should never happen
    assert(false);
  }
}

// dpi ----------------------------------------------------------------------

word rotateRight(word value, uint rotateNum) {
  uint lsbs = value & ((1 << rotateNum) - 1);
  return (value >> rotateNum) | (lsbs << (32 - rotateNum));
}

word arithShift(word value, uint shiftNum) {
  word msb = value & 0x80000000;
  // TODO: change variable name
  word temp = msb;
  for (int i = 0; i < shiftNum; i++) {
    temp = temp >> 1;
    temp = temp + msb;
  }
  return temp | (value >> shiftNum);
}

uint shiftByConstant(uint shiftPart) {
  // integer specified by bits 7-4
  return shiftPart >> 4;
}

uint shiftByRegister(arm state, uint shiftPart) {
  // Rs (register) can be any general purpose register except the PC
  uint rs = shiftPart >> 4;
  // bottom byte of value in Rs specifies the amount to be shifted
  return state.registers[rs] & 0x000000FF;
}

uint rightCarryOut(word value, uint shiftNum) {
  return (value >> (shiftNum - 1)) & 0x00000001;
}

tuple_t *barrelShifter(arm state, word value, uint shiftPart) {
  // bit to determine what to shift by
  bool shiftByConst = shiftPart & 0x01;
  // number to shift by
  uint shiftNum = shiftByConst ? shiftByConstant(shiftPart)
                               : shiftByRegister(state, shiftPart);
  // bits that specify the shift operation
  enum Shift shiftType = (shiftPart & 0x06) >> 1;
  // tuple for the result and the carry out bit
  tuple_t *output = (tuple_t *)malloc(sizeof(tuple_t));
  check_ptr(output, "Not enough memory!");
  word result;
  word carryOut;
  // TODO: change variable name
  word temp;
  switch (shiftType) {
  case LSL:
    temp = value << (shiftNum - 1);
    carryOut = temp & 0x80000000;
    result = temp << 1;
    break;
  case LSR:
    carryOut = rightCarryOut(value, shiftNum);
    result = value >> shiftNum;
    break;
  case ASR:
    carryOut = rightCarryOut(value, shiftNum);
    result = arithShift(value, shiftNum);
    break;
  case ROR:
    carryOut = rightCarryOut(value, shiftNum);
    result = rotateRight(value, shiftNum);
    break;
  default:
    // no other shift instruction
    // should never happen
    assert(false);
  }
  output->result = result;
  output->carryOut = carryOut;
  return output;
}

tuple_t *opRegister(arm state, uint op2) {
  // register that holds the value to be shifted
  uint rm = op2 & 0x00F;
  // value to be shifted
  word value = state.registers[rm];
  // bits indicating the shift instruction
  uint shiftPart = op2 >> 4;
  return barrelShifter(state, value, shiftPart);
}

tuple_t *opImmediate(arm state, uint op2) {
  // 8-bit immediate value zero-extended to 32 bits
  word imm = op2 & 0x0FF;
  // number to rotate by
  uint rotateNum = (op2 >> 8) * 2;
  // tuple for the result and the carry out bit
  tuple_t *output = (tuple_t *)malloc(sizeof(tuple_t));
  check_ptr(output, "Not enough memory!");
  // result of the rotation operation
  output->result = rotateRight(imm, rotateNum);
  // carry out for CSPR flag
  output->carryOut = rightCarryOut(imm, rotateNum);
  return output;
}

void setCPSR(arm state, word result, uint carryOut) {
  // set to the logical value of bit 31 of the result
  word n = result & 0x80000000;
  // set only if the result is all zeros
  word z;
  if (!result) {
    z = 1 << 29;
  } else {
    z = 0;
  }
  word c = carryOut << 31;
  // unaffected
  uint v = state.registers[CPSR] & 0x10000000;
  // updated flag bits
  state.registers[CPSR] = n | z | c | v;
}

void dpi(arm state, word instruction) {
  // decode
  const uint i = (instruction & 0x02000000) >> 25;
  // instruction to execute
  enum Opcode opcode = (instruction & 0x01E00000) >> 20;
  const uint s = (instruction & 0x00100000) >> 20;
  // op1 is always the contents of register Rn
  const uint rn = (instruction & 0x000F0000) >> 16;
  // destination register
  const uint rd = (instruction & 0x0000F000) >> 12;
  // second operand
  word op2 = instruction & 0x00000FFF;
  // first operand
  word op1 = state.registers[rn];
  // if i is set, op2 is an immediate const, otherwise it's a shifted register
  tuple_t *output = i ? opImmediate(state, op2) : opRegister(state, op2);
  op2 = output->result;
  word carryOut = output->carryOut;
  // execution
  word result;
  switch (opcode) {
  case AND:
    result = op1 & op2;
    state.registers[rd] = result;
    break;
  case EOR:
    result = op1 ^ op2;
    state.registers[rd] = result;
    break;
  case SUB:
    result = op1 - op2;
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
  case TST:
    result = op1 & op2;
    break;
  case TEQ:
    result = op1 ^ op2;
    break;
  case CMP:
    result = op1 - op2;
    break;
  case ORR:
    result = op1 | op2;
    state.registers[rd] = op1 | op2;
    break;
  case MOV:
    state.registers[rd] = op2;
    break;
  }

  // if s is set then the CPSR flags should be updated
  if (s) {
    setCPSR(state, result, carryOut);
  }
  free(output);
}

// end of dpi ---------------------------------------------------------------

// execution of the multiply instruction
void multiply(arm *state, word instruction) {
  // extraction of information from the instruction;
  int destination = (instruction & MULT_RDEST_MASK) >> MULT_RDEST_SHIFT;
  int regS = (instruction & MULT_REG_S_MASK) >> MULT_REG_S_SHIFT;
  int regM = instruction & MULT_REG_M_MASK;

  // Initial execution of instruction
  int result = state->registers[regM] * state->registers[regS];

  // obtain value from Rn and add to result if Accumulate is set
  if (instruction & ACCUMULATE_FLAG) {
    int regN = (instruction & MULT_REG_N_MASK) >> MULT_REG_N_SHIFT;
    result += state->registers[regN];
  }
  // update CPSR flags if S (bit 20 in instruction) is set
  if (instruction & UPDATE_CPSR) {
    state->registers[CPSR] |= (result & CPSR_N);
    if (!result)
      state->registers[CPSR] |= CPSR_Z;
  }
}

// execution of the branch instruction
void branch(arm *state, word instruction) {
  // extraction of information
  int offset = instruction & BRANCH_OFFSET_MASK;
  int signBit = offset & BRANCH_SIGN_BIT;

  // shift, sign extension and addition of offset onto current address
  state->registers[PC] +=
      (offset << CURRENT_INSTRUCTION_SHIFT) |
      (signBit ? NEGATIVE_SIGN_EXTEND : POSITIVE_SIGN_EXTEND);
}

/* Takes in the ARM binary file's name and returns an ARM state pointer with
memory and register
pointers on heap, where memory is of size MEM_LIMIT bytes */
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

  if (!checkCond(state, instruction)) {
    return;
  }

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
