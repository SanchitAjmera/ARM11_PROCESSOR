#include <stdlib.h>
#inlcude "../emulate_constants.h"
#include "../emulate_util.h"
#inlcude "../../common/constants.h"
#inlcude "../../common/util.h"
#include "emulate_decode.h"

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
