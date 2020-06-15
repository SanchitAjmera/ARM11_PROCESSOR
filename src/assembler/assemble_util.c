#include "assemble_util.h"
#include "../common/constants.h"
#include "../common/util.h"
#include "assemble_constants.h"
#include "file_lines.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Returns respective int value; -1 for failure */
int lookup(const pair_t table[], const int size, const char *key) {
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i].key, key)) {
      return table[i].value;
    }
  }
  return LOOKUP_FAILURE;
}

/* Performs the first scan on a file adding labels to the symbol table,
    as well as expressions. Returns an array of strings that
    represent each line, stripped of the newline \n,
    and stores expressions in their string form at the end of the array. */
void scanFile(FILE *armFile, symbol_table *symbolTable, file_lines *output) {
  assert(armFile != NULL && symbolTable != NULL && output != NULL);

  // Will be used to store expressions found during the scan
  file_lines *expressions = newFileLines();

  /* Scan file for labels and expressions */
  char *line = malloc(LINE_CHAR_LIM);
  while (fgets(line, LINE_CHAR_LIM, armFile) != NULL) {
    // Iterate through chars in line
    bool isLabel = false;
    char *lineCopy = strptr(line);
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == ':') { // Line is a label
        char *label = strtok(line, ":");
        symbol *labelSymbol = malloc(sizeof(*labelSymbol));
        validatePtr(labelSymbol, "NULL pointer.");
        symbol temp = {strptr(label), LABEL, 0,
                       .body.address = output->lineCount * WORD_SIZE_BYTES};
        *labelSymbol = temp;
        addSymbol(symbolTable, labelSymbol);
        isLabel = true;
        break;                         // Line contains only one label
      } else if (lineCopy[i] == '=') { // Line contains an =0x expression
        char *expression =
            strtok(lineCopy + i - 1, " ],\n"); // Pulls expression out
        // =0x........ is only added to end of file if greater than 0xFF
        if (parseImmediate(expression + 1) > MAX_BYTE) {
          addLine(expressions, expression);
        }
        break; // Each line contains up to one expression. Remove if incorrect
      }
    }
    if (!isLabel) {
      char *lineStripped;
      if ((lineStripped = strtok(line, "\n"))) {
        addLine(output, lineStripped); // Adds line stripped of \n
      }
    }
    free(lineCopy);
  }
  free(line);

  /* Now that we have added all lines to output, we can add all expressions
     to the symbol table and calculate their address using lineCount */
  for (int i = 0; i < expressions->lineCount; i++) {
    char *expr = expressions->lines[i];
    word address = (output->lineCount + i) * WORD_SIZE_BYTES;
    symbol *exprSymbol = malloc(sizeof(*exprSymbol));
    validatePtr(exprSymbol, "Null pointer.");
    symbol temp = {strptr(expr), LABEL, 0, .body.address = address};
    *exprSymbol = temp;
    addSymbol(symbolTable, exprSymbol);
  }

  // Add expressions to the end of the file
  addLines(output, expressions->lines, expressions->lineCount);

  freeFileLines(expressions);
}

/* Performs the second pass on fileLines */
void parseLines(file_lines *in, symbol_table *symbolTable, FILE *out) {
  assert(out);
  for (int i = 0; i < in->lineCount; i++) {
    char *line = in->lines[i];
    char *rest = NULL;

    char *fields[5];
    int fieldCount = 0;
    char *token = strtok_r(line, " ,", &rest);
    while (token != NULL) {
      if (rest[0] == ' ') {
        REMOVE_FIRST_CHAR(rest);
        continue;
      }
      fields[fieldCount++] = token;
      if (rest[0] == '[') { // If the next token starts with a [
        token = strtok_r(rest, "]", &rest);
      } else {
        token = strtok_r(NULL, " ,", &rest);
      }
    }

    // Stores the current instruction's information in struct
    instruction instr = {
        fields[0], lookup(opcodeTable, PREDEFINED_SYMBOLS_COUNT, fields[0]),
        fields + 1, fieldCount - 1, i * WORD_SIZE_BYTES};
    symbol *instrSymbol = getSymbol(symbolTable, instr.opcode);
    assert(instrSymbol != NULL);
    word binLine;
    if (instrSymbol->type == INSTR) {
      binLine = instrSymbol->body.assembleFunc(symbolTable, instr);
    } else {
      // Parse expression
      binLine = parseImmediate(in->lines[i] + 1);
    }

    fwrite(&binLine, sizeof(word), 1, out);
  }
}

/* Converts string to integer for both denary and hex constants */
uint parseImmediate(char *op2) {
  // PRE: # has been removed from <#expression> (op2)
  if (op2[0] == '-') {
    REMOVE_FIRST_CHAR(op2);
  }
  if (strlen(op2) > 2) {
    if (op2[0] == '0' && op2[1] == 'x') {
      return (uint)strtol(op2, NULL, HEX_BASE);
    }
  }
  return (uint)atoi(op2);
}

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
void exitOverflow(uint num, const uint max) {
  if (num > max) {
    fprintf(stderr, "Number cannot be represented.\n");
    // TODO: change to errorExit();
    exit(EXIT_FAILURE);
  }
}

/* Calculates the rotation amount to fit an immediate constant in 8 bits */
static word calcRotatedImm(word imm) {
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
static word parseOperand2Imm(char **op2) {
  uint imm = parseImmediate(REMOVE_FIRST_CHAR(op2[0]));
  exitOverflow(imm, MAX_NUM);
  if (imm > MAX_BYTE) {
    return calcRotatedImm(imm);
  }
  return imm;
}

/* Calculates binary representation of a register operand2
including registers with shifts attached */
static word parseOperand2Reg(char **op2, uint args) {
  uint rm = REM_INT(op2[0]);
  if (args < SHIFT_NO_ARGS) {
    // No shift type - shift of 0
    return rm;
  }
  Shift shiftType = parseShiftType(op2[1]);
  if (IS_IMMEDIATE(op2[2])) {
    uint shiftNum = parseImmediate(REMOVE_FIRST_CHAR(op2[2]));
    return (shiftNum << 7) | (shiftType << 5) | rm;
  }
  uint rs = REM_INT(op2[2]);
  // Rs can be any general purpose register except the PC
  assert(rs != PC);
  return (rs << RS_SHIFT) | (shiftType << SHIFT_TYPE_SHIFT) |
         SHIFT_BY_REG_HARDCODE | rm;
}

/* Checks type of operand 2 (imm/reg) and calls corresponding parser */
static word parseOperand2(char **op2, uint args) {
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

/* Provides assembly function for 'mla' and 'mul' instructions and
returns the corresponding ARM-binary based on the instruction arguments */
word assembleMultiply(symbol_table *symbolTable, instruction input) {
  // Defining the components of the instruction
  word rd = REM_INT(input.fields[0]) << MULT_RDEST_SHIFT;
  word rm = REM_INT(input.fields[1]);
  word rs = REM_INT(input.fields[2]) << MULT_REG_S_SHIFT;

  // Initialising for 'mul', may be updated if 'mla'
  word rn = 0;
  word accumulate = 0;

  // Set rn and A for an 'accumulate' input
  if (input.mnemonic == MLA) {
    rn = REM_INT(input.fields[3]) << MULT_REG_N_SHIFT;
    accumulate = ACCUMULATE_FLAG;
  }
  // S is set to 0 so no need to explicitly write it
  // Bits 4-7 are hardcoded as 1001
  return ALWAYS | accumulate | rd | rn | rs | MULT_HARDCODE | rm;
}

/* Converts all intructions related to a branch instruction, including
both conditional and unconditional, to the corresponding ARM-binary */
word assembleBranch(symbol_table *symbolTable, instruction input) {
  // The 'b' instruction is always executed
  // Otherwise the condition of the branch will be the letters following 'b'
  word cond =
      input.mnemonic == B
          ? ALWAYS
          : lookup(condTable, COND_TABLE_SIZE, REMOVE_FIRST_CHAR(input.opcode))
                << COND_SHIFT;

  word currentAddress = input.currentAddress;
  char *target = input.fields[0];

  // A hashtag denotes a constant address, otherwise the address needs
  // found from the symbol table
  word targetAddress = IS_IMMEDIATE(target)
                           ? REM_INT(target)
                           : getSymbol(symbolTable, target)->body.address;
  // Calculates the offset with the pipeline effect considered
  word offset = (targetAddress - currentAddress - PIPELINE_OFFSET) >>
                CURRENT_INSTRUCTION_SHIFT;

  return cond | BRANCH_HARDCODE | (offset & BRANCH_OFFSET_MASK);
}

/* Removes bracketing around string
converts remaining strings into unsigned int values
returns array containing register address and expression address */
word *remBracket(const char *string) {
  word *addresses = malloc(sizeof(word) * 4);
  char *unbracketed = strptr(string + 1);
  // separator
  char *delim = ", ";
  // Gets Rn
  char *token = strtok(unbracketed, delim);
  // gets address of register rn
  if (token[0] == 'r') { // rn could either be a register or immediate const
    REMOVE_FIRST_CHAR(token);
  }
  addresses[0] = parseImmediate(token);
  token = strtok(NULL, delim);
  // If expression exists in address
  if (token != NULL) {
    char firstLetter = token[0];
    char secondLetter = REMOVE_FIRST_CHAR(token)[0];
    // Set U and I bits
    addresses[3] = firstLetter == 'r' ? 1 : 0;
    addresses[2] = secondLetter == '-' ? 0 : 1;
    addresses[1] = parseImmediate(token);
  }
  free(unbracketed);
  return addresses;
}

/* Decodes the address provided within the instruction struct */
static SDTIOperation SDTIparser(char **fields, uint field_count) {
  // returns correct enum corresponding to decoded address
  if (field_count == POST_COUNT) {
    return POST_RN_EXP;
  } else if (strstr(fields[1], ",")) {
    return PRE_RN_EXP;
  } else if (strstr(fields[1], "r")) {
    return PRE_RN;
  } else {
    return NUMERIC_CONST;
  }
}

/* Assembly function for all store and load instructions */
word assembleSDTI(symbol_table *symbolTable, instruction input) {
  // TODO: (WIP) I refactored your `remBracket` & added var `addresses` - Alex
  word *addresses = remBracket(input.fields[1]);

  // Decoding address type
  SDTIOperation operation = SDTIparser(input.fields, input.field_count);
  // Load bit
  word l = (input.mnemonic == LDR) ? (1 << SDTI_L_SHIFT) : 0;
  // PRE/POST-INDEXING bits
  word p = (operation == POST_RN_EXP) ? 0 : (1 << SDTI_P_SHIFT);
  // Base register Rn
  word Rn = addresses[0] << SDTI_RN_SHIFT;
  // Source/ dest register Rd
  word Rd = REM_INT(input.fields[0]) << SDTI_RD_SHIFT;
  // Offsets
  word offset;
  // Up bit
  word u = 1 << SDTI_U_SHIFT;
  // Immediate offsets
  word i = 0;
  // Switch case for different address types
  switch (operation) {
  case POST_RN_EXP:
    // Offset
    i = IS_IMMEDIATE(input.fields[2]) ? 0 : 1 << SDTI_I_SHIFT;
    offset = REM_INT(input.fields[2]);
    break;
  case PRE_RN:
    // Offset is 0
    offset = 0;
    break;
  case PRE_RN_EXP:
    // Offset
    // TODO: check if this is null? - Alex
    offset = addresses[1];
    u = addresses[2] << SDTI_U_SHIFT;
    i = addresses[3] << SDTI_I_SHIFT;
    break;
  case NUMERIC_CONST:
    // Check if expression can fit inside a mov function
    if (parseImmediate(input.fields[1] + 1) <= SDTI_EXP_BOUND) {
      input.opcode = "mov";
      input.mnemonic = MOV;
      free(addresses);
      return assembleDPI(symbolTable, input);
    } else {
      // offset
      offset = getSymbol(symbolTable, input.fields[1])->body.address -
               PIPELINE_OFFSET;
      offset -= input.currentAddress;
      // Base register Rn
      Rn = PC << SDTI_RN_SHIFT;
    }
    break;
  default:
    // This should never happen, fields were most likely parsed wrong
    assert(false);
    break;
  }
  // Freeing memory for the register address and expression address
  free(addresses);
  // Returning constructed instruction
  return ALWAYS | SDTI_HARDCODE | i | p | u | l | Rn | Rd | offset;
}
