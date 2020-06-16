#include "emulate_execute.h"
#include "../../common/constants.h"
#include "../../common/util.h"
#include "../emulate_constants.h"
#include "../emulate_util.h"

/* Constant shift if operand 2 is a register (bit 4 = 0) */
static uint shiftByConstant(uint shiftPart) {
  // integer specified by bits 7-4
  return shiftPart >> GET_SHIFT_CONSTANT;
}

/* Shift by value in another register (bit 4 = 1) */
static uint shiftByRegister(arm_t *state, uint shiftPart) {
  // Rs (register) can be any general purpose register except the PC
  uint rs = shiftPart >> GET_RS_SHIFT;
  // bottom byte of value in Rs specifies the amount to be shifted
  return state->registers[rs] & LEAST_BYTE_MASK;
}

/* Returns the result of an arithmetic right shift on value by shiftNum */
static word arithShift(word value, uint shiftNum) {
  word msb = value & MSB_MASK;
  word msbs = msb;
  for (int i = 0; i < shiftNum; i++) {
    msbs = msbs >> 1;
    msbs = msbs + msb;
  }
  return msbs | (value >> shiftNum);
}

/* Circular bitwise right rotation of value by rotateNum */
static word rotateRight(word value, uint rotateNum) {
  uint lsbs = value & ((1 << rotateNum) - 1);
  return (value >> rotateNum) | (lsbs << (WORD_SIZE - rotateNum));
}

/* Calculates the carry out bit from a logical left shift */
static uint leftCarryOut(word value, uint shiftNum) {
  if (shiftNum == 0) {
    return NO_ROTATION;
  }
  return (value << (shiftNum - 1)) >> (WORD_SIZE - 1);
}

/* Calculates carry out bit from a right rotation or shift */
static uint rightCarryOut(word value, uint shiftNum) {
  if (shiftNum == 0) {
    return NO_ROTATION;
  }
  return (value >> (shiftNum - 1)) & LSB_MASK;
}

/* Calculates the shift type and amount, return value and carry outs from a
supported shift operation */
static operation_t *barrelShifter(arm_t *state, word value, uint shiftPart) {
  // bit to determine what to shift by
  bool shiftByReg = shiftPart & LSB_MASK;
  // number to shift by
  uint shiftNum = shiftByReg ? shiftByRegister(state, shiftPart)
                             : shiftByConstant(shiftPart);
  // bits that specify the shift operation
  Shift shiftType = (shiftPart & SHIFT_TYPE_MASK) >> GET_SHIFT_TYPE;
  // tuple for the result and the carry out bit
  operation_t *shiftedOp2 = (operation_t *)malloc(sizeof(operation_t));
  errorExit(validatePtr(shiftedOp2, "Not enough memory!"));
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

/* Sets up barrel shifter to perform shift ops as op2 is a register */
static operation_t *opRegister(arm_t *state, uint op2) {
  // register that holds the value to be shifted
  uint rm = op2 & LEAST_NIBBLE_MASK;
  // value to be shifted
  word value = state->registers[rm];
  // bits indicating the shift instruction (bits 11-4)
  uint shiftPart = op2 >> GET_SHIFT_INSTRUCTION;
  return barrelShifter(state, value, shiftPart);
}

/* Returns the immediate value rotated right by the given rotation amount */
static operation_t *opImmediate(arm_t *state, uint op2) {
  // 8-bit immediate value zero-extended to 32 bits
  word imm = op2 & LEAST_BYTE_MASK;
  // number to rotate by
  uint rotateNum = (op2 >> GET_ROTATION_NUM) * ROTATION_FACTOR;
  // struct for the result and the carry out bit
  operation_t *shiftedOp2 = (operation_t *)malloc(sizeof(operation_t));
  errorExit(validatePtr(shiftedOp2, "Not enough memory!"));
  // result of the rotation operation
  shiftedOp2->result = rotateRight(imm, rotateNum);
  // carry out for CSPR flag
  shiftedOp2->carryOut = rightCarryOut(imm, rotateNum);
  return shiftedOp2;
}

/* Carry out from arithmetic opertaion */
static word getCarryOut(word op1, word op2, bool isAddition) {
  if (isAddition) {
    return (op1 <= UINT32_MAX - op2) ? 0 : 1;
  }
  // pre: op1 - op2
  return op1 < op2 ? 0 : 1;
}

/* Sets the values of the CPSR depending on the result of an operation */
static void setCPSR(arm_t *state, word result, uint carryOut) {
  // set to the logical value of bit 31 of the result
  word n = result & CPSR_N_MASK;
  // set only if the result is all zeros
  word z = result ? 0 : CPSR_Z_MASK;
  // carry out from the instruction
  word c = carryOut ? CPSR_C_MASK : 0;
  // v is unaffected
  word v = state->registers[CPSR] & CPSR_V_MASK;
  // updated flag bits
  state->registers[CPSR] = n | z | c | v;
}

/* Executes supported Data Processing instructions from opcode */
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

/* function for checking if word is within MEMORY_CAPACITY
   prints error if memory is out of bounds */
static bool checkValidAddress(word address) {
  if (address > MEMORY_CAPACITY) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", address);
    return false;
  }
  return true;
}

/* Stores address, found in source register Rd, into the the memory */
static void store(arm_t *state, word sourceReg, word baseReg) {
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

/* Function which loads address inside base register Rn into the memory */
static void load(arm_t *state, word destReg, word sourceAddr) {
  // check for making sure address is within bounds of MEMORY_CAPACITY
  if (checkValidAddress(sourceAddr)) {
    // value inside base register
    word value = getWord(state->memory + sourceAddr, NOT_BIG_ENDIAN);
    // laoding value into destination register Rd
    state->registers[destReg] = value;
  }
}

/* Executes all supported Single Data Transfer instructions */
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

/* Executes all supported Multiply instructions */
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

/* Pipeline flush required for a branch instruction */
void flushPipeline(arm_t *state) {
  state->fetched = 0;
  state->decoded.instruction = 0;
  state->decoded.isSet = false;
}

/* Execution of a branch instruction */
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

// checks the instruction condition with the CPSR flags
bool checkCond(arm_t *state, word instruction) {
  // CPSR flag bits
  word cpsr = state->registers[CPSR];
  uint n = GET_CPSR_N(cpsr);
  uint z = GET_CPSR_Z(cpsr);
  uint v = GET_CPSR_V(cpsr);
  Cond cond = GET_CPSR_FLAGS(instruction);
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
