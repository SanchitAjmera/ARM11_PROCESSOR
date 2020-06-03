#include "emulate_util.h"
#include "constants.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void validatePtr(const void *ptr, const char *error_msg) {
  if (ptr == NULL) {
    printf("Error: %s\n", error_msg);
    exit(EXIT_FAILURE);
  }
}

uint shiftByConstant(uint shiftPart) {
  // integer specified by bits 7-4
  return shiftPart >> GET_SHIFT_CONSTANT_SHIFT;
}

uint shiftByRegister(arm_t *state, uint shiftPart) {
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

operation_t *barrelShifter(arm_t *state, word value, uint shiftPart) {
  // bit to determine what to shift by
  bool shiftByReg = shiftPart & LSB_MASK;
  // number to shift by
  uint shiftNum = shiftByReg ? shiftByRegister(state, shiftPart)
                             : shiftByConstant(shiftPart);
  // bits that specify the shift operation
  enum Shift shiftType = (shiftPart & SHIFT_TYPE_MASK) >> GET_SHIFT_TYPE;
  // tuple for the result and the carry out bit
  operation_t *shiftedOp2 = (operation_t *)malloc(sizeof(operation_t));
  validatePtr(shiftedOp2, "Not enough memory!");
  word result;
  // carry out from a right shift operation
  uint carryOut = rightCarryOut(value, shiftNum);
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
  shiftedOp2->result = result;
  shiftedOp2->carryOut = carryOut;
  return shiftedOp2;
}

operation_t *opRegister(arm_t *state, uint op2) {
  // register that holds the value to be shifted
  uint rm = op2 & LEAST_NIBBLE_MASK;
  // value to be shifted
  word value = state->registers[rm];
  // bits indicating the shift instruction (bits 11-4)
  uint shiftPart = op2 >> GET_SHIFT_INSTRUCTION;
  return barrelShifter(state, value, shiftPart);
}

operation_t *opImmediate(arm_t *state, uint op2) {
  // 8-bit immediate value zero-extended to 32 bits
  word imm = op2 & LEAST_BYTE_MASK;
  // number to rotate by
  uint rotateNum = (op2 >> GET_ROTATION_NUM) * ROTATION_FACTOR;
  // struct for the result and the carry out bit
  operation_t *shiftedOp2 = (operation_t *)malloc(sizeof(operation_t));
  validatePtr(shiftedOp2, "Not enough memory!");
  // result of the rotation operation
  shiftedOp2->result = rotateRight(imm, rotateNum);
  // carry out for CSPR flag
  shiftedOp2->carryOut = rightCarryOut(imm, rotateNum);
  return shiftedOp2;
}

// carry out from arithmetic opertaion
word getCarryOut(word op1, word op2, bool isAddition) {
  if (isAddition) {
    return (op1 <= UINT32_MAX - op2) ? 0 : 1;
  }
  // pre: op1 - op2
  return op1 < op2 ? 0 : 1;
}

void setCPSR(arm_t *state, word result, uint carryOut) {
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

void executeDPI(arm_t *state, dp_t *decoded) {
  // if i is set, op2 is an immediate const, otherwise it's a shifted register
  operation_t *shiftedOp2 = decoded->i ? opImmediate(state, decoded->op2)
                                       : opRegister(state, decoded->op2);
  word op1 = decoded->op1;
  word op2 = shiftedOp2->result;
  uint rd = decoded->rd;
  uint carryOut = shiftedOp2->carryOut;
  word result;
  switch (decoded->opcode) {
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
    carryOut = getCarryOut(op1, op2, NOT_ADDITION);
    state->registers[rd] = result;
    break;
  case RSB:
    result = op2 - op1;
    carryOut = getCarryOut(op2, op1, NOT_ADDITION);
    state->registers[rd] = result;
    break;
  case ADD:
    result = op1 + op2;
    carryOut = getCarryOut(op1, op2, IS_ADDITION);
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
    carryOut = getCarryOut(op1, op2, NOT_ADDITION);
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
  if (decoded->s) {
    setCPSR(state, result, carryOut);
  }
  free(shiftedOp2);
  free(decoded);
}

// function for checking if word is within MEMORY_CAPACITY
// prints error if memory is out of bounds
bool checkValidAddress(word address) {
  if (address > MEMORY_CAPACITY) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", address);
    return false;
  }
  return true;
}

// function which stores address inside source register Rd into the the memory
void store(arm_t *state, word sourceReg, word baseReg) {
  // check for making sure address is within bounds of MEMORY_CAPACITY
  if (!checkValidAddress(baseReg)) {
    return;
  }
  // value inside source register Rd
  word value = state->registers[sourceReg];
  // storing value within memory located by address inside base register
  for (int i = 0; i < WORD_SIZE_BYTES; i++) {
    state->memory[baseReg + i] = value >> BYTE * i;
  }
}

// function which loads address inside base register Rn into the memory
void load(arm_t *state, word destReg, word sourceAddr) {
  // check for making sure address is within bounds of MEMORY_CAPACITY
  if (checkValidAddress(sourceAddr)) {
    // value inside base register
    word value = getWord(state->memory + sourceAddr, NOT_BIG_ENDIAN);
    // laoding value into destination register Rd
    state->registers[destReg] = value;
  }
}

void executeSDTI(arm_t *state, sdt_t *decoded) {
  uint l = decoded->l;
  uint rd = decoded->rd;
  word offset = decoded->offset;
  operation_t *shiftedOffset =
      decoded->i ? opRegister(state, offset) : opImmediate(state, offset);
  // no carry out from this instruction
  offset = shiftedOffset->result;
  free(shiftedOffset);
  word *baseReg = state->registers + decoded->rn;
  // offset if added if u is set, subtracted if not
  offset = decoded->u ? offset : -offset;
  if (decoded->p) {
    // pre-indexing
    l ? load(state, rd, *baseReg + offset)
      : store(state, rd, *baseReg + offset);
  } else {
    // post-indexing
    l ? load(state, rd, *baseReg) : store(state, rd, *baseReg);
    *baseReg += offset;
  }
  free(decoded);
}

void executeMultiply(arm_t *state, multiply_t *decoded) {
  int regS = decoded->regS;
  int regM = decoded->regM;
  // initial execution of instruction
  int result = state->registers[regM] * state->registers[regS];
  // obtain value from Rn and add to result if Accumulate (A) is set
  if (decoded->a) {
    result += state->registers[decoded->regN];
  }
  // update CPSR flags if S (bit 20 in instruction) is set
  if (decoded->s) {
    // same as Data Processing but no carry out
    setCPSR(state, result, GET_CPSR_C(state->registers[CPSR]));
  }
  state->registers[decoded->destination] = result;
  free(decoded);
}

// pipeline flush required for a branch instruction
void flushPipeline(arm_t *state) {
  state->fetched = 0;
  state->decoded.instruction = 0;
  state->decoded.isSet = false;
}

void executeBranch(arm_t *state, branch_t *decoded) {
  flushPipeline(state);
  int offset = decoded->offset;
  // extracting information from instruction
  int signBit = offset & BRANCH_SIGN_BIT;
  // shift, sign extension and addition of offset onto current address in PC
  state->registers[PC] +=
      ((offset << CURRENT_INSTRUCTION_SHIFT) |
       (signBit ? NEGATIVE_SIGN_EXTEND : POSITIVE_SIGN_EXTEND));
  free(decoded);
}

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEMORY_CAPACITY bytes */
void initArm(arm_t *state, const char *fname) {
  // load binary file into memory
  byte *memory = (byte *)calloc(MEMORY_CAPACITY, sizeof(byte));
  validatePtr(memory, "Not enough memory.\n");
  // file handling
  FILE *binFile = fopen(fname, "rb");
  validatePtr(binFile, "File could not be opened\n");
  fseek(binFile, SEEK_SET, SEEK_END);
  long fileSize = ftell(binFile);
  rewind(binFile);
  // Asserts that fread read the whole file
  assert(fread(memory, 1, fileSize, binFile) == fileSize);
  fclose(binFile);
  // initialise registers
  word *registers = (word *)calloc(NUM_REGISTERS, sizeof(word));

  // construct ARM state
  state->memory = memory;
  state->registers = registers;
  flushPipeline(state);
}

word getWord(byte *startAddress, bool isBigEndian) {
  word w = 0;
  for (int i = 0; i < WORD_SIZE_BYTES; i++) {
    uint byte_index = isBigEndian ? (WORD_SIZE_BYTES - 1 - i) : i;
    w += startAddress[i] << BYTE * byte_index;
  }
  return w;
}

void fetch(arm_t *state) {
  word memoryOffset = state->registers[PC];
  state->registers[PC] += WORD_SIZE_BYTES;
  state->fetched = getWord(state->memory + memoryOffset, NOT_BIG_ENDIAN);
}

dp_t *decodeDPI(arm_t *state, word instruction) {
  dp_t *decoded = malloc(sizeof(dp_t));
  // decoded of code
  decoded->i = (instruction & DPI_I_MASK) >> DPI_I_SHIFT;
  // instruction to execute
  decoded->opcode = (instruction & DPI_OPCODE_MASK) >> DPI_OPCODE_SHIFT;
  // if s (bit 20) is set then the CPSR flags should be updated
  decoded->s = UPDATE_CPSR(instruction);
  // op1 is always the contents of register Rn
  decoded->rn = (instruction & DPI_RN_MASK) >> DPI_RN_SHIFT;
  // destination register
  decoded->rd = (instruction & DPI_RD_MASK) >> DPI_RD_SHIFT;
  // first operand
  decoded->op1 = state->registers[decoded->rn];
  // second operand
  decoded->op2 = instruction & DPI_OP2_MASK;
  return decoded;
}

multiply_t *decodeMultiply(arm_t *state, word instruction) {
  multiply_t *decoded = malloc(sizeof(multiply_t));
  decoded->a = instruction & ACCUMULATE_FLAG;
  decoded->s = UPDATE_CPSR(instruction);
  decoded->destination = (instruction & MULT_RDEST_MASK) >> MULT_RDEST_SHIFT;
  decoded->regN = (instruction & MULT_REG_N_MASK) >> MULT_REG_N_SHIFT;
  decoded->regS = (instruction & MULT_REG_S_MASK) >> MULT_REG_S_SHIFT;
  decoded->regM = instruction & MULT_REG_M_MASK;
  return decoded;
}

sdt_t *decodeSDTI(arm_t *state, word instruction) {
  sdt_t *decoded = malloc(sizeof(sdt_t));
  decoded->i = (instruction & SDTI_I_MASK) >> SDTI_I_SHIFT;
  decoded->p = (instruction & SDTI_P_MASK) >> SDTI_P_SHIFT;
  decoded->u = (instruction & SDTI_U_MASK) >> SDTI_U_SHIFT;
  decoded->l = (instruction & SDTI_L_MASK) >> SDTI_L_SHIFT;
  decoded->rn = (instruction & SDTI_RN_MASK) >> SDTI_RN_SHIFT;
  decoded->rd = (instruction & SDTI_RD_MASK) >> SDTI_RD_SHIFT;
  decoded->offset = instruction & SDTI_OFFSET_MASK;
  return decoded;
}

branch_t *decodeBranch(arm_t *state, word instruction) {
  branch_t *decoded = malloc(sizeof(branch_t));
  decoded->offset = instruction & BRANCH_OFFSET_MASK;
  return decoded;
}

void decode(arm_t *state, decoded_t *decoded) {
  word instruction = state->fetched;
  state->decoded.instruction = instruction;
  // halt instruction
  if (state->fetched == 0) {
    return;
  }
  if (BITS_SET(instruction, DECODE_BRANCH_MASK, DECODE_BRANCH_EXPECTED)) {
    state->decoded.instructionType = BR;
    decoded->branch = decodeBranch(state, instruction);
  } else if (BITS_SET(instruction, DECODE_SDT_MASK, DECODE_SDT_EXPECTED)) {
    state->decoded.instructionType = SDTI;
    decoded->sdt = decodeSDTI(state, instruction);
  } else if (BITS_SET(instruction, DECODE_MULT_MASK, DECODE_MULT_EXPECTED)) {
    state->decoded.instructionType = MULT;
    decoded->multiply = decodeMultiply(state, instruction);
  } else if (BITS_SET(instruction, DECODE_DPI_MASK, DECODE_DPI_EXPECTED)) {
    state->decoded.instructionType = DPI;
    decoded->dp = decodeDPI(state, instruction);
  }
  state->decoded.isSet = true;
}

// checks the instruction condition with the CPSR flags
bool checkCond(arm_t *state, word instruction) {
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

void execute(arm_t *state, decoded_t *decoded) {
  word instruction = state->decoded.instruction;
  if (!state->decoded.isSet || !checkCond(state, instruction)) {
    return;
  }
  switch (state->decoded.instructionType) {
  // TODO: consider removing instruction from the paramter of each execution
  case BR:
    executeBranch(state, decoded->branch);
    break;
  case DPI:
    executeDPI(state, decoded->dp);
    break;
  case SDTI:
    executeSDTI(state, decoded->sdt);
    break;
  case MULT:
    executeMultiply(state, decoded->multiply);
    break;
  case IGNR:
    // the instruction is ignored
    break;
  default:
    // no other instructions
    // should never happen
    assert(false);
  }
}
