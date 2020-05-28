#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Main assumptions about the emulator/ ARM
#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

// constants for masks
// Constants for Multiply
#define SDTI_I_MASK 0x02000000
#define SDTI_P_MASK 0x01000000
#define SDTI_U_MASK 0x00800000
#define SDTI_L_MASK 0x00100000
#define SDTI_RN_MASK 0x000F0000
#define SDTI_RD_MASK 0x0000F000
#define SDTI_OFFSET_MASK 0x00000FFF
#define SDTI_I_SHIFT 25
#define SDTI_P_SHIFT 24
#define SDTI_U_SHIFT 23
#define SDTI_L_SHIFT 20
#define SDTI_RN_SHIFT 16
#define SDTI_RD_SHIFT 12

// an unassigned 32 bit int for a word
typedef uint32_t word;

enum Register { PC = 15, CPSR = 16 };

// condition suffixes for their codes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };

typedef struct {
  // ARM machine memory
  uint8_t *memory;
  // 0-12 general purpose , 13 SP, 14 LR, 15 PCM 16 CPSR
  word *registers;
} arm;

// CHECK IF CORRECTLY ASSIGNED MEMORY
void ptrValidate(const void *pointer, char *error) {
  if (pointer == NULL) {
    printf("Error: %s\n", error);
    exit(EXIT_FAILURE);
  }
}

// function to check conditions
// Parameter 1: code for Cond
// Parameter 2: current state of arm
bool checkCond(word instruction, arm state) {
  // CPSR FLAG BITS - WE USE 1,2,4,8 to extract the 1,2,3,4th bit in the cpsr
  // address
  unsigned int n = (state.registers[CPSR] & 0x80000000) >> 31;
  unsigned int z = (state.registers[CPSR] & 0x40000000) >> 30;
  unsigned int c = (state.registers[CPSR] & 0x20000000) >> 29;
  unsigned int v = (state.registers[CPSR] & 0x10000000) >> 28;
  unsigned int cond = instruction >> 28;
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
    ;
  case AL:
    return true;
  default:
    return false;
  }
}

// -Alex's DPI ---------------------------------------------------------------

word rotateRight(word value, unsigned int rotateNum) {
  unsigned int lsbs = value & ((1 << rotateNum) - 1);
  return (value >> rotateNum) | (lsbs << (WORD_SIZE - rotateNum));
}

word arithShift(word value, unsigned int shiftNum) {
  word msb = value & MSB_MASK;
  // TODO: change variable name
  word msbs = msb;
  for (int i = 0; i < shiftNum; i++) {
    msbs = msbs >> 1;
    msbs = msbs + msb;
  }
  return msbs | (value >> shiftNum);
}

unsigned int shiftByConstant(uint shiftPart) {
  // integer specified by bits 7-4
  // #define GET_SHIFT_CONSTANT_SHIFT 3
  return shiftPart >> 3;
}

unsigned int shiftByRegister(arm *state, unsigned int shiftPart) {
  // Rs (register) can be any general purpose register except the PC
  unsigned int rs = shiftPart >> GET_RS_SHIFT;
  // bottom byte of value in Rs specifies the amount to be shifted
  return state->registers[rs] & LEAST_BYTE;
}

unsigned int leftCarryOut(word value, unsigned int shiftNum) {
  return (value << (shiftNum - 1)) >> (WORD_SIZE - 1);
}

unsigned int rightCarryOut(word value, asigned int shiftNum) {
  return (value >> (shiftNum - 1)) & LSB_MASK;
}

tuple_t *barrelShifter(arm *state, word value, signed int shiftPart) {
  // bit to determine what to shift by
  bool shiftByReg = shiftPart & LSB_MASK;
  // number to shift by
  signed int shiftNum = shiftByReg ? shiftByRegister(state, shiftPart)
                                   : shiftByConstant(shiftPart);
  // bits that specify the shift operation
  enum Shift shiftType = (shiftPart & SHIFT_TYPE_MASK) >> GET_SHIFT_TYPE_SHIFT;
  // tuple for the result and the carry out bit
  tuple_t *output = (tuple_t *)malloc(sizeof(tuple_t));
  check_ptr(output, "Not enough memory!");
  word result;
  word carryOut = rightCarryOut(value, shiftNum);
  switch (shiftType) {
  case LSL:
    carryOut = leftCarryOut(value, shiftNum);
    result = value << shiftNum;
    break;
  case LSR:
    result = value >> shiftNum;
    break;
  case ASR:
    result = arithShift(value, shiftNum);
    break;
  case ROR:
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

tuple_t *opRegister(arm *state, unsigned int op2) {
  // register that holds the value to be shifted
  unsigned int rm = op2 & LSN_MASK;
  // value to be shifted
  word value = state->registers[rm];
  // bits indicating the shift instruction (bits 11-4)
  unsigned int shiftPart = op2 >> GET_SHIFT_INSTRUCTION_SHIFT;
  return barrelShifter(state, value, shiftPart);
}

tuple_t *opImmediate(arm *state, unassigned int op2) {
  // 8-bit immediate value zero-extended to 32 bits
  word imm = op2 & LEAST_BYTE;
  // number to rotate by
  unsigned int rotateNum = (op2 >> GET_ROTATE_SHIFT) * ROTATION_FACTOR;
  // tuple for the result and the carry out bit
  tuple_t *output = (tuple_t *)malloc(sizeof(tuple_t));
  check_ptr(output, "Not enough memory!");
  // result of the rotation operation
  output->result = rotateRight(imm, rotateNum);
  // carry out for CSPR flag
  output->carryOut = rightCarryOut(imm, rotateNum);
  return output;
}

uint getCarryOut(word op1, word op2, word result) {
  unassigned int carryOut = 0;
  word signBit = MSB_MASK;
  // overflow occurs iff the operands have the same sign and the result has ...
  // ... the opposite sign
  if ((op1 & signBit) == (op2 & signBit)) {
    carryOut = (op1 & signBit) != (result & signBit);
  }
  return carryOut;
}

void setCPSR(arm *state, word result, unsigned int carryOut) {
  // set to the logical value of bit 31 of the result
  word n = result & MSB_MASK;
  // set only if the result is all zeros
  word z = result ? 0 : CPSR_Z;
  // carry out from the instruction
  word c = carryOut ? SET_CPSR_C : 0;
  // unaffected
  unsigned int v = state->registers[CPSR] & CPSR_V_MASK;
  // updated flag bits
  state->registers[CPSR] = n | z | c | v;
}

//-Single Data Tranfer Instructions function ----------------------------------

void sdti(arm state, word instruction) {
  if (!checkCond(instruction & 0xF0000000, state)) {
    return;
  }
  // parts of the instruction
  unsigned int i = (instruction & SDTI_I_MASK) >> SDTI_I_SHIFT;
  unsigned int p = (instruction & SDTI_P_MASK) >> SDTI_P_SHIFT;
  unsigned int u = (instruction & SDTI_U_MASK) >> SDTI_U_SHIFT;
  unsigned int l = (instruction & SDTI_L_MASK) >> SDTI_L_SHIFT;
  unsigned int rn = (instruction & SDTI_RN_MASK) >> SDTI_RN_SHIFT;
  unsigned int rd = (instruction & SDTI_RD_MASK) >> SDTI_RD_SHIFT;
  word offset = (instruction & SDTI_OFFSET_MASK);

  // Immediate Offset

  offset = i ? opRegister(state, offset) : opImmediate(state, offset);

  // p doesn't change contents of base register for this exercise
  if (p) {
    // if flag is set then (pre-Indexing) and simply transfer data

  } else {

    if (u) {
      // offset is added to base register if u is set
      rn += offset;

    } else {
      // subtract offset from base register
      rn -= offset;
    }
    // trasfer data
  }

  if (l) {
    // word is loaded from memory
    // word ldr is the word stored within the source register rd
    // TODO: find way to load word from source register into variable ldr
  } else {
    // word str is the word from the updated base register rn
    // this word is stored within the destination/source register rd
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
    // function for data processing instructions
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
