#ifndef EMULATE_UTIL_H
#define EMULATE_UTIL_H

#include "../common/constants.h"
#include "../common/util.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// ARM instruction set
typedef enum { DPI, MULT, BR, SDTI, IGNR } InstructionType;

// Struct for the result and carry out from shift/arithmetic operation
typedef struct {
  word result;
  uint carryOut;
} operation_t;

// Struct for the decoding of Data Processing instructions
typedef struct {
  uint i;
  OpcodeDP opcode;
  uint s;
  uint rn;
  uint rd;
  word op1;
  word op2;
} dp_t;

// Struct for the decoding of Multiply instructions
typedef struct {
  uint a;
  uint s;
  int destination;
  int regN;
  int regS;
  int regM;
} multiply_t;

// Struct for the decoding of Single Data Transfer instructions
typedef struct {
  uint i;
  uint p;
  uint u;
  uint l;
  uint rn;
  uint rd;
  word offset;
} sdt_t;

// Struct for the decoding of Branch instructions
typedef struct {
  int offset;
} branch_t;

// Union for the decoded instruction types
typedef union {
  dp_t *dp;
  multiply_t *multiply;
  sdt_t *sdt;
  branch_t *branch;
} decoded_t;

// Struct for state of the instruction
typedef struct {
  bool isSet;
  word instruction;
  InstructionType instructionType;
} instructionState_t;

// Struct for the state of the ARM
typedef struct {
  byte *memory;
  // 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR
  word *registers;
  word fetched;
  instructionState_t decoded;
} arm_t;

/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
extern void initArm(arm_t *state, const char *fname);

#endif
