#include "emulate_util.h"
#include "constants.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void check_ptr(const void *ptr, const char *error_msg) {
  if (ptr == NULL) {
    printf("Error: %s\n", error_msg);
    exit(EXIT_FAILURE);
  }
}

uint shiftByConstant(uint shiftPart) {
  // integer specified by bits 7-4
  return shiftPart >> GET_SHIFT_CONSTANT_SHIFT;
}

uint shiftByRegister(arm *state, uint shiftPart) {
  // Rs (register) can be any general purpose register except the PC
  uint rs = shiftPart >> GET_RS_SHIFT;
  // bottom byte of value in Rs specifies the amount to be shifted
  return state->registers[rs] & LEAST_BYTE_MASK;
}

word arithShift(word value, uint shiftNum) {
  word msb = value & MSB_MASK;
  word msbs = msb;
  for (int i = 0; i < shiftNum; i++) {
    msbs = msbs >> 1;
    msbs = msbs + msb;
  }
  return msbs | (value >> shiftNum);
}

word rotateRight(word value, uint rotateNum) {
  uint lsbs = value & ((1 << rotateNum) - 1);
  return (value >> rotateNum) | (lsbs << (WORD_SIZE - rotateNum));
}

uint leftCarryOut(word value, uint shiftNum) {
  if (shiftNum == 0) {
    return NO_ROTATION;
  }
  return (value << (shiftNum - 1)) >> (WORD_SIZE - 1);
}

uint rightCarryOut(word value, uint shiftNum) {
  if (shiftNum == 0) {
    return NO_ROTATION;
  }
  return (value >> (shiftNum - 1)) & LSB_MASK;
}

operation_t *barrelShifter(arm *state, word value, uint shiftPart) {
  // bit to determine what to shift by
  bool shiftByReg = shiftPart & LSB_MASK;
  // number to shift by
  uint shiftNum = shiftByReg ? shiftByRegister(state, shiftPart)
                             : shiftByConstant(shiftPart);
  // bits that specify the shift operation
  enum Shift shiftType = (shiftPart & SHIFT_TYPE_MASK) >> GET_SHIFT_TYPE;
  // tuple for the result and the carry out bit
  operation_t *output = (operation_t *)malloc(sizeof(operation_t));
  check_ptr(output, "Not enough memory!");
  word result;
  // carry out from a right shift operation
  word carryOut = rightCarryOut(value, shiftNum);
  switch (shiftType) {
  case LSL:
    // carry out from a left shift operation
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

operation_t *opRegister(arm *state, uint op2) {
  // register that holds the value to be shifted
  uint rm = op2 & LEAST_NIBBLE_MASK;
  // value to be shifted
  word value = state->registers[rm];
  // bits indicating the shift instruction (bits 11-4)
  uint shiftPart = op2 >> GET_SHIFT_INSTRUCTION;
  return barrelShifter(state, value, shiftPart);
}

operation_t *opImmediate(arm *state, uint op2) {
  // 8-bit immediate value zero-extended to 32 bits
  word imm = op2 & LEAST_BYTE_MASK;
  // number to rotate by
  uint rotateNum = (op2 >> GET_ROTATION_NUM) * ROTATION_FACTOR;
  // struct for the result and the carry out bit
  operation_t *output = (operation_t *)malloc(sizeof(operation_t));
  check_ptr(output, "Not enough memory!");
  // result of the rotation operation
  output->result = rotateRight(imm, rotateNum);
  // carry out for CSPR flag
  output->carryOut = rightCarryOut(imm, rotateNum);
  return output;
}

// carry out from arithmetic opertaion
word getCarryOut(word op1, word op2, bool isAddition) {
  if (isAddition) {
    return (op1 <= UINT32_MAX - op2) ? 0 : 1;
  }
  // pre: op1 - op2
  return op1 < op2 ? 0 : 1;
}

void setCPSR(arm *state, word result, word carryOut) {
  // set to the logical value of bit 31 of the result
  word n = result & CPSR_N_MASK;
  // set only if the result is all zeros
  word z = result ? 0 : CPSR_Z_MASK;
  // carry out from the instruction
  word c = carryOut ? SET_CPSR_C : 0;
  // v is unaffected
  word v = state->registers[CPSR] & CPSR_V_MASK;
  // updated flag bits
  state->registers[CPSR] = n | z | c | v;
}

void executedpi(arm *state, word instruction) {
  // extraction of code
  const uint i = (instruction & DPI_I_MASK) >> DPI_I_SHIFT;
  // instruction to execute
  enum Opcode opcode = (instruction & DPI_OPCODE_MASK) >> DPI_OPCODE_SHIFT;
  // op1 is always the contents of register Rn
  const uint rn = (instruction & DPI_RN_MASK) >> DPI_RN_SHIFT;
  // destination register
  const uint rd = (instruction & DPI_RD_MASK) >> DPI_RD_SHIFT;
  // second operand
  word op2 = instruction & DPI_OP2_MASK;
  // first operand
  word op1 = state->registers[rn];
  // if i is set, op2 is an immediate const, otherwise it's a shifted register
  // TODO: change variable name
  operation_t *output = i ? opImmediate(state, op2) : opRegister(state, op2);
  op2 = output->result;
  word carryOut = output->carryOut;
  // execution
  word result;
  switch (opcode) {
  case AND:
    result = op1 & op2;
    state->registers[rd] = result;
    break;
  case EOR:
    result = op1 ^ op2;
    state->registers[rd] = result;
    break;
  case SUB:
    result = op1 - op2;
    // subtraction, so isAddition is false
    carryOut = getCarryOut(op1, op2, false);
    state->registers[rd] = result;
    break;
  case RSB:
    result = op2 - op1;
    // subtraction, so isAddition is false
    carryOut = getCarryOut(op2, op1, false);
    state->registers[rd] = result;
    break;
  case ADD:
    result = op1 + op2;
    // addition, so isAddition is true
    carryOut = getCarryOut(op1, op2, true);
    state->registers[rd] = result;
    break;
  case TST:
    result = op1 & op2;
    break;
  case TEQ:
    result = op1 ^ op2;
    break;
  case CMP:
    result = op1 - op2;
    // subtraction, so isAddition is false
    carryOut = getCarryOut(op1, op2, false);
    break;
  case ORR:
    result = op1 | op2;
    state->registers[rd] = result;
    break;
  case MOV:
    result = op2;
    state->registers[rd] = result;
    break;
  default:
    // no other instructions
    // should never happen
    assert(false);
  }

  // if s (bit 20) is set then the CPSR flags should be updated
  if (UPDATE_CPSR(instruction)) {
    setCPSR(state, result, carryOut);
  }
  free(output);
}

// function for checking if word is within MEMORY_CAPACITY
// ADDRESS_SIZE is taken away from MEMORY_CAPACITY as address must be
// ADDRESS_SIZE less than MEMORY_CAPACITY in order for word to be read
bool checkValidAddress(word address) {
  if (address > MEMORY_CAPACITY) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", address);
    return false;
  }
  return true;
}

// function which transfers data from one register to another
void store(arm *state, word sourceReg, word destAddr) {
  if (!checkValidAddress(destAddr)) {
    return;
  }
  word value = state->registers[sourceReg];
  for (int i = 0; i < WORD_SIZE_BYTES; i++) {
    state->memory[destAddr + i] = value >> BYTE * i;
  }
}

void load(arm *state, word destReg, word sourceAddr) {
  if (checkValidAddress(sourceAddr)) {
    word value = get_word(state->memory + sourceAddr);
    state->registers[destReg] = value;
  }
}

void executesdti(arm *state, word instruction) {
  // Components of the instruction
  // Immediate Offset
  uint i = (instruction & SDTI_I_MASK) >> SDTI_I_SHIFT;
  // Pre/Post indexing bit
  uint p = (instruction & SDTI_P_MASK) >> SDTI_P_SHIFT;
  // Up bit
  uint u = (instruction & SDTI_U_MASK) >> SDTI_U_SHIFT;
  // Load/Store bit
  uint l = (instruction & SDTI_L_MASK) >> SDTI_L_SHIFT;
  // Base Register
  uint rn = (instruction & SDTI_RN_MASK) >> SDTI_RN_SHIFT;
  // Source/Destination register
  uint rd = (instruction & SDTI_RD_MASK) >> SDTI_RD_SHIFT;
  // Offset
  word offset = (instruction & SDTI_OFFSET_MASK);

  // Immediate Offset
  operation_t *output =
      i ? opRegister(state, offset) : opImmediate(state, offset);
  offset = output->result;

  // Because PRE-INDEXING doesn't change the value of the base register rn
  // the data will always be transferred regardless of the indexing bit p.
  // transfering Data:

  // POST-INDEXING is set
  offset = u ? offset : -offset;
  word destAddr = state->registers[rn];
  if (p) {
    // indexing base regsiter Rn according to Up bit
    l ? load(state, rd, destAddr + offset)
      : store(state, rd, destAddr + offset);
  } else {
    l ? load(state, rd, destAddr) : store(state, rd, destAddr);
    state->registers[rn] += offset;
  }
}

void executeMultiply(arm *state, word instruction) {
  // extraction of information from the instruction;
  int destination = (instruction & MULT_RDEST_MASK) >> MULT_RDEST_SHIFT;
  int regS = (instruction & MULT_REG_S_MASK) >> MULT_REG_S_SHIFT;
  int regM = instruction & MULT_REG_M_MASK;

  // initial execution of instruction
  int result = state->registers[regM] * state->registers[regS];

  // obtain value from Rn and add to result if Accumulate is set
  if (instruction & ACCUMULATE_FLAG) {
    int regN = (instruction & MULT_REG_N_MASK) >> MULT_REG_N_SHIFT;
    result += state->registers[regN];
  }
  // update CPSR flags if S (bit 20 in instruction) is set
  if (UPDATE_CPSR(instruction)) {
    // same as Data Processing but no carry out
    setCPSR(state, result, GET_CPSR_C(state->registers[CPSR]));
  }
  state->registers[destination] = result;
}

void executeBranch(arm *state, word instruction) {
  // flush pipeline
  state->fetched = 0;
  state->decoded.is_set = false;
  state->decoded.instr = 0;

  // extraction of information
  int offset = instruction & BRANCH_OFFSET_MASK;
  int signBit = offset & BRANCH_SIGN_BIT;

  // shift, sign extension and addition of offset onto current address
  state->registers[PC] +=
      ((offset << CURRENT_INSTRUCTION_SHIFT) |
       (signBit ? NEGATIVE_SIGN_EXTEND : POSITIVE_SIGN_EXTEND));
}

bool checkCond(arm *state, word instruction) {
  // CPSR flag bits
  word cpsr = state->registers[CPSR];
  uint n = GET_CPSR_N(cpsr);
  uint z = GET_CPSR_Z(cpsr);
  uint v = GET_CPSR_V(cpsr);
  enum Cond cond = GET_CPSR_FLAGS(instruction);
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

bool checkCond(arm *state, word instruction) {
  // CPSR flag bits
  word cpsr = state->registers[CPSR];
  uint n = GET_CPSR_N(cpsr);
  uint z = GET_CPSR_Z(cpsr);
  uint v = GET_CPSR_V(cpsr);
  enum Cond cond = GET_CPSR_FLAGS(instruction);
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

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEMORY_CAPACITY bytes */
void init_arm(arm *state, const char *fname) {
  /* load binary file into memory */
  byte *memory = (byte *)calloc(MEMORY_CAPACITY, sizeof(byte));
  check_ptr(memory, "Not enough memory.\n");
  // file handling
  FILE *bin_obj = fopen(fname, "rb");
  check_ptr(bin_obj, "File could not be opened\n");
  fseek(bin_obj, SEEK_SET, SEEK_END);
  long file_size = ftell(bin_obj);
  rewind(bin_obj);
  /* Asserts that fread read the whole file */
  assert(fread(memory, 1, file_size, bin_obj) == file_size);
  fclose(bin_obj);
  /* initialise registers */
  word *registers = (word *)calloc(NUM_REGISTERS, sizeof(word));

  /* construct ARM state */
  state->memory = memory;
  state->registers = registers;
  state->fetched = 0;
  state->decoded.is_set = false;
}

word get_word(byte *start_addr) {
  word w = 0;
  for (int i = 0; i < WORD_SIZE_BYTES; i++) {
    w += start_addr[i] << BYTE * (i);
  }
  return w;
}

word get_word_big_end(byte *start_addr) {
  word w = 0;
  for (int i = 0; i < WORD_SIZE_BYTES; i++) {
    w += start_addr[i] << BYTE * (WORD_SIZE_BYTES - 1 - i);
  }
  return w;
}

void fetch(arm *state) {
  word memory_offset = state->registers[PC];
  state->registers[PC] += WORD_SIZE_BYTES;
  state->fetched = get_word(state->memory + memory_offset);
}

void decode(arm *state) {
  word instruction = state->fetched;
  state->decoded.instr = instruction;
  if (state->fetched == 0) {
    return;
  }

  if (BITS_SET(instruction, DECODE_BRANCH_MASK, DECODE_BRANCH_EXPECTED)) {
    state->decoded.instrSet = BR;
  } else if (BITS_SET(instruction, DECODE_SDT_MASK, DECODE_SDT_EXPECTED)) {
    state->decoded.instrSet = SDTI;
  } else if (BITS_SET(instruction, DECODE_MULT_MASK, DECODE_MULT_EXPECTED)) {
    state->decoded.instrSet = MULT;
  } else if (BITS_SET(instruction, DECODE_DPI_MASK, DECODE_DPI_EXPECTED)) {
    state->decoded.instrSet = DPI;
  }
  state->decoded.is_set = true;
}

void execute(arm *state) {
  if (state->decoded.is_set == false) {
    // printf("null exec\n");
    return;
  }
  tuple_instruction instructionTuple = state->decoded;
  InstructionSet instructionSet = instructionTuple.instrSet;
  word instruction = instructionTuple.instr;

  if (checkCond(state, instruction)) {
    switch (instructionSet) {
    case BR:
      executeBranch(state, instruction);
      break;
    case DPI:
      executedpi(state, instruction);
      break;
    case SDTI:
      executesdti(state, instruction);
      break;
    case MULT:
      executeMultiply(state, instruction);
      break;
    case IGNR:
      break;
    default:
      assert(false);
    }
  }
}
