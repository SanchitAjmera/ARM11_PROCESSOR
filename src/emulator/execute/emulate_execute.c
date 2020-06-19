#include "emulate_execute.h"
#include "../../common/constants.h"
#include "../../common/util.h"
#include "../emulate_constants.h"
#include "../emulate_util.h"
#include "../fetch/emulate_fetch.h"
#include "emulate_execute_dpi.h"
#include <stdio.h>

/* function for checking if word is within MEMORY_CAPACITY
   prints error if memory is out of bounds */
static void checkValidAddress(word address) {
  if (address > MEMORY_CAPACITY) {
    errorExit(MEM_OVERFLOW);
  }
}

/* Stores address, found in source register Rd, into the the memory */
static void store(arm_t *state, word sourceReg, word baseReg) {
  // check for making sure address is within bounds of MEMORY_CAPACITY
  checkValidAddress(baseReg);
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
  checkValidAddress(sourceAddr);
  // value inside base register
  word value = getWord(state->memory + sourceAddr, NOT_BIG_ENDIAN);
  // laoding value into destination register Rd
  state->registers[destReg] = value;
}

/* Executes all supported Single Data Transfer instructions */
static void executeSDTI(arm_t *state, sdt_t *decoded) {
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
static void executeMultiply(arm_t *state, multiply_t *decoded) {
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
static void executeBranch(arm_t *state, branch_t *decoded) {
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
static bool checkCond(arm_t *state, word instruction) {
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
    errorExit(UNEXPECTED_CASE);
    return false;
  }
}

void execute(arm_t *state, decoded_t *decoded) {
  word instruction = state->decoded.instruction;
  if (!state->decoded.isSet || !checkCond(state, instruction)) {
    return;
  }
  switch (state->decoded.instructionType) {
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
    errorExit(UNEXPECTED_CASE);
  }
}
