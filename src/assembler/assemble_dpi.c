#include "../common/constants.h"
#include "../common/util.h"
#include "assemble_constants.h"
#include "assemble_util.h"
#include "file_lines.h"

/* Calculates shift type enum from string */
static Shift parseShiftType(const char *shift) {
  return lookup(shiftTable, SHIFT_TABLE_SIZE, shift);
}

/* Returns circular bitwise left rotation of input num by rotateNum */
static word rotateLeft(word value, uint rotateNum) {
  uint msbs = value & ~((1 << (WORD_SIZE - rotateNum)) - 1);
  return (value << rotateNum) | (msbs >> (WORD_SIZE - rotateNum));
}

/* Exits program if num exceeds the highest representable value */
extern void exitOverflow(uint num, const uint max) {
  if (num > max) {
    // fprintf(stderr, "Number cannot be represented.\n");
    errorExit(MEM_OVERFLOW);
  }
}

/* Calculates the rotation amount to fit an immediate constant in 8 bits */
extern static word calcRotatedImm(word imm) {
  // PRE: imm can be represented by WORD_SIZE bits
  uint mask = 1;
  uint rotation = 0;
  for (int i = 0; i < WORD_SIZE; i++) {
    if (mask & imm) {
      rotation = WORD_SIZE - i;
      break;
    }
    mask = mask << 1;
  }
  // Rotation must be even
  if (rotation % ROTATION_FACTOR != 0) {
    rotation++;
  }
  imm = rotateLeft(imm, rotation);
  exitOverflow(imm, MAX_BYTE);
  rotation = rotation / ROTATION_FACTOR;
  return (rotation << GET_ROTATION_NUM) | imm;
}

/* Calculates immediate value, including any rotation required to
fit number into 8 bits */
extern static word parseOperand2Imm(char **op2) {
  word imm = (word)parseImmediate(REMOVE_FIRST_CHAR(op2[0]));
  exitOverflow(imm, MAX_NUM);
  if (imm > MAX_BYTE) {
    imm = calcRotatedImm(imm);
  }
  return imm;
}

/* Calculates binary representation of a register operand2
including registers with shifts attached */
extern static word parseOperand2Reg(char **op2, uint args) {
  uint rm = REM_INT(op2[0]);
  if (args < SHIFT_NO_ARGS) {
    // No shift type (shift of 0)
    return rm;
  }
  Shift shiftType = parseShiftType(op2[1]);
  if (IS_IMMEDIATE(op2[2])) {
    return (parseImmediate(REMOVE_FIRST_CHAR(op2[2])) << SHIFT_NUM_SHIFT) |
           (shiftType << SHIFT_TYPE_SHIFT) | rm;
  }
  uint rs = REM_INT(op2[2]);
  // Rs can be any general purpose register except the PC
  if (rs == PC) {
    errorExit(UNEXPECTED_CASE);
  }
  return (rs << RS_SHIFT) | (shiftType << SHIFT_TYPE_SHIFT) |
         SHIFT_BY_REG_HARDCODE | rm;
}

/* Checks type of operand 2 (imm/reg) and calls corresponding parser */
extern static word parseOperand2(char **op2, uint args) {
  // 8 bit immediate value - <#expression>
  // Decimal or hexadecimal ("#n" or “#0x...”)
  if (IS_IMMEDIATE(op2[0])) {
    return parseOperand2Imm(op2);
  }
  // Shifted register - Rm <shiftname> {<register> or <#expression>}
  return parseOperand2Reg(op2, args);
}

/* Provides assembly conversion for supported data processing instructions */
word assembleDPI(symbol_table *symbolTable, instruction input) {
  word opcode = input.mnemonic;
  word s = 0;
  word rn = 0;
  word rd = 0;
  char **operand2;
  uint args;
  char *imm;

  // andeq r0, r0, r0
  if (opcode == ANDEQ) {
    return 0x00000000;
  }

  // instruction: mov
  // syntax: mov Rd, <Operand2>
  else if (opcode == MOV) {
    imm = input.fields[1];
    rd = REM_INT(input.fields[0]);
    operand2 = input.fields + 1;
    args = input.field_count - 1;
  }

  // lsl Rn, <#expression>
  else if (opcode == LSL_SPECIAL) {
    opcode = MOV;
    imm = input.fields[0];
    char *rdCopy = input.fields[0];
    rd = REM_INT(input.fields[0]);
    char *ops[SHIFT_NO_ARGS] = {rdCopy, "lsl", input.fields[1]};
    args = SHIFT_NO_ARGS;
    operand2 = ops;
  }

  // instructions: tst, teq, cmp
  // syntax: <opcode> Rn, <Operand2>

  else if (opcode == TST || opcode == TEQ || opcode == CMP) {
    imm = input.fields[1];
    // COND flags should be updated (s bit is set)
    s = 1;
    rn = REM_INT(input.fields[0]);
    operand2 = input.fields + 1;
    args = input.field_count - 1;
  }

  // instructions: and, eor, sub, rsb, add, orr
  // syntax: <opcode> Rd, Rn, <Operand2>
  else {
    imm = input.fields[2];
    rn = REM_INT(input.fields[1]);
    rd = REM_INT(input.fields[0]);
    operand2 = input.fields + 2;
    args = input.field_count - 2;
  }

  opcode = opcode << DPI_OPCODE_SHIFT;
  s = s << DPI_S_SHIFT;
  rn = rn << DPI_RN_SHIFT;
  rd = rd << DPI_RD_SHIFT;
  word i = IS_IMMEDIATE(imm) ? (1 << DPI_I_SHIFT) : 0;
  word op2 = parseOperand2(operand2, args);
  return ALWAYS | i | opcode | s | rn | rd | op2;
}
