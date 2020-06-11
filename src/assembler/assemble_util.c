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

/* Performs the first scan on a file adding labels to the symbol table,
    as well as expressions. Returns an array of strings that
    represent each line, stripped of the newline \n,
    and stores expressions in their string form at the end of the array. */
void scanFile(FILE *armFile, symbol_table *symbolTable, file_lines *output) {
  assert(armFile != NULL && symbolTable != NULL && output != NULL);

  /* Will be used to store expressions found during the scan */
  file_lines *expressions = newFileLines();

  /* Scan file for labels and expressions */
  char line[LINE_CHAR_LIM];
  while (fgets(line, LINE_CHAR_LIM, armFile) != NULL) {
    /* iterate through chars in line */
    bool isLabel = false;
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == ':') { // Line is a label
        char *label = strtok(line, ":");
        symbol labelSymbol = {strptr(label), LABEL,
                              .body.address =
                                  output->lineCount * WORD_SIZE_BYTES};
        addSymbol(symbolTable, labelSymbol);
        isLabel = true;
        break;                     // Line contains only one label
      } else if (line[i] == '=') { // Line contains an =0x expression
        char *expression =
            strtok(line + i - 1, " ],\n"); // Pulls expression out
        addLine(expressions, expression);
        break; // Each line contains up to one expression. Remove if incorrect
      }
    }
    if (isLabel == false) {
      addLine(output, strtok(line, "\n")); // Adds line stripped of \n
    }
  }

  /* Now that we have added all lines to output, we can add all expressions
     to the symbol table and calculate their address using lineCount */
  for (int i = 0; i < expressions->lineCount; i++) {
    char *expr = expressions->lines[i];
    word address = (output->lineCount + i) * WORD_SIZE_BYTES;
    symbol exprSymbol = {strptr(expr), LABEL, .body.address = address};
    addSymbol(symbolTable, exprSymbol);
  }

  addLines(output, expressions->lines, expressions->lineCount);

  freeFileLines(expressions);
}

/*  Performs the second pass on fileLines */
void parseLines(file_lines *in, symbol_table *symbolTable, FILE *out) {
  for (int i = 0; i < in->lineCount; i++) {
    char *line = in->lines[i];
    char *rest = NULL;

    char *fields[5];
    int fieldCount = 0;
    char *token = strtok_r(line, " ,", &rest);
    while (token != NULL) {
      fields[fieldCount++] = token;
      if (rest[0] == '[') { // If the next token starts with a [
        token = strtok_r(rest, "]", &rest);
      } else {
        token = strtok_r(NULL, " ,", &rest);
      }
    }

    instruction instr = {fields[0], fields + 1, fieldCount - 1,
                         i * WORD_SIZE_BYTES};
    symbol *instrSymbol = getSymbol(symbolTable, instr.opcode);
    assert(instrSymbol != NULL);
    assert(instrSymbol->type == INSTR);
    word binLine = instrSymbol->body.assembleFunc(symbolTable, instr);
    printf("output: %x\n", binLine);

    fwrite(&binLine, sizeof(word), 1, out);

    for (int j = 0; j < fieldCount; j++) {
      printf("token: %s\n", fields[j]);
    }
    printf("\n");
  }
}

// removes first character and returns integer from string
word rem(const char *string) { return atoi(++string); }

// returns respective int value; -1 for failure
int lookup(const pair_t table[], const int size, const char *key) {
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i].key, key)) {
      return table[i].value;
    }
  }
  return LOOKUP_FAILURE;
}

enum Opcode parseDPIOpcode(char *mnemonic) {
  return lookup(opcodeTable, OPCODE_TABLE_SIZE, mnemonic);
}

uint parseImmediate(const char *op2) {
  if (strlen(op2) > 2) {
    if (op2[1] == '0' && op2[2] == 'x') {
      // TODO: check NULL end point works correctly
      return (uint)strtol(op2, NULL, HEX_BASE);
    }
  }
  return (uint)rem(op2);
}

enum Shift parseShiftType(const char *shift) {
  return lookup(shiftTable, SHIFT_TABLE_SIZE, shift);
}

word rotateLeft(word value, uint rotateNum) {
  uint msbs = value & ~((1 << rotateNum) - 1);
  return (value << rotateNum) | (msbs >> (WORD_SIZE - rotateNum));
}

word calcRotatedImm(word imm) {
  // PRE: imm can be represented by WORD_SIZE bits
  uint mask = 1;
  uint rotation = 0;
  for (int i = 0; i < WORD_SIZE; i++) {
    if (mask & imm) {
      rotation = WORD_SIZE - i;
    }
    mask = mask << 1;
  }
  imm = rotateLeft(imm, rotation);
  if (imm > MAX_BYTE) {
    fprintf(stderr, "Number cannot be represented.");
    exit(EXIT_FAILURE);
  }
  return (rotation << 8) | imm;
}

word parseOperand2Imm(char **op2) {
  uint imm = parseImmediate(op2[0]);
  if (OVERFLOW(imm)) {
    fprintf(stderr, "Number cannot be represented.");
    exit(EXIT_FAILURE);
  }
  if (imm > MAX_BYTE) {
    return calcRotatedImm(imm);
  }
  return imm;
}

word parseOperand2Reg(char **op2) {
  uint rm = rem(op2[0]);
  enum Shift shiftType = parseShiftType(op2[1]);
  if (IS_IMMEDIATE(op2[2])) {
    uint shiftNum = parseImmediate(op2[2]);
    return (shiftNum << 8) | (shiftType << 5) | rm;
  }
  uint rs = rem(op2[2]);
  // Rs can be any general purpose register except the PC
  assert(rs != PC);
  return (rs << 9) | (shiftType << 5) | (1 << 4) | rm;
}

word parseOperand2(char **op2) {
  // 8 bit immediate value - <#expression>
  // decimal or hexadecimal ("#n" or “#0x...”)
  if (IS_IMMEDIATE(op2[0])) {
    return parseOperand2Imm(op2);
  }
  // shifted register - Rm <shiftname> {<register> or <#expression>}
  return parseOperand2Reg(op2);
}

word assembleDPI(symbol_table *symbolTable, instruction input) {
  word opcode = parseDPIOpcode(input.opcode);
  word s = 0;
  word rn = 0;
  word rd = 0;
  char **operand2;
  char *imm;

  // instruction: mov
  // syntax: mov Rd, <Operand2>
  if (opcode == MOV) {
    imm = input.fields[1];
    rd = rem(input.fields[0]);
    operand2 = input.fields + 1;
  }

  // instructions: tst, teq, cmp
  // syntax: <opcode> Rn, <Operand2>

  else if (opcode == TST || opcode == TEQ || opcode == CMP) {
    imm = input.fields[1];
    // COND flags should be updated (s bit is set)
    s = 1;
    rn = rem(input.fields[0]);
    operand2 = input.fields + 1;
  }

  // instructions: and, eor, sub, rsb, add, orr
  // syntax: <opcode> Rd, Rn, <Operand2>
  else {
    imm = input.fields[2];
    rn = rem(input.fields[1]);
    rd = rem(input.fields[0]);
    operand2 = input.fields + 2;
  }

  opcode = opcode << DPI_OPCODE_SHIFT;
  s = s << DPI_S_SHIFT;
  rn = rn << DPI_RN_SHIFT;
  rd = rd << DPI_RD_SHIFT;
  word i = IS_IMMEDIATE(imm) ? (1 << DPI_I_SHIFT) : 0;
  word op2 = parseOperand2(operand2);
  return ALWAYS | i | opcode | s | rn | rd | op2;
}

/*Provides assembly function for 'mla' and 'mul' instructions and
returns the corresponding ARM-binary based on the instruction arguments*/
word assembleMultiply(symbol_table *symbolTable, instruction input) {
  // Defining the components of the instruction
  word rd = rem(input.fields[0]) << MULT_RDEST_SHIFT;
  word rm = rem(input.fields[1]);
  word rs = rem(input.fields[2]) << MULT_REG_S_SHIFT;

  // Initialising for 'mul', may be updated if 'mla'
  word rn = 0;
  word accumulate = 0;

  // set rn and A for an 'accumulate' input
  if (!strcmp("mla", input.opcode)) {
    rn = rem(input.fields[3]) << MULT_REG_N_SHIFT;
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
  word cond = !strcmp("b", input.opcode)
                  ? ALWAYS
                  : lookup(condTable, COND_TABLE_SIZE, ++(input.opcode))
                        << COND_SHIFT;

  word currentAddress = input.currentAddress;
  char *target = input.fields[0];

  // A hashtag denotes a constant address, otherwise the address needs
  // found from the symbol table
  word targetAddress = IS_IMMEDIATE(target)
                           ? rem(target)
                           : getSymbol(symbolTable, target)->body.address;
  // Calculates the offset with the pipeline effect considered
  word offset = (targetAddress - currentAddress - 8) >> 2;
  return cond | BRANCH_HARDCODE | (offset & BRANCH_OFFSET_MASK);
}

/* removes bracketing around string
converts remaining strings into unsigned int values
returns array containing register address and expression address */
word *remBracket(char *string) {
  word *addresses = malloc(sizeof(*addresses));
  int length = strlen(string);
  char unbracketed[length - 2];
  // removing brackets
  for (int i = 1; i < length - 1; i++) {
    unbracketed[i - 1] = string[i];
  }
  // separator
  char *delim = ",";
  // gets Rn
  char *token = strtok(unbracketed, delim);
  // gets address of register rn
  addresses[0] = atoi(++token);
  token = strtok(NULL, delim);
  // if expression exists in address
  if (token != NULL) {
    addresses[1] = atoi(++token);
  }
  return addresses;
}

// this function decodes the address provided within the instruction struct
SDTIOperation SDTIparser(char **fields, uint field_count) {
  // returns correct enum corresponding to decoded address
  if (field_count == 3) {
    return POST_RN_EXP;
  } else if (strstr(fields[1], ",")) {
    return PRE_RN_EXP;
  } else if (strstr(fields[1], "r")) {
    return PRE_RN;
  } else {
    return NUMERIC_CONST;
  }
}

word assembleSDTI(symbol_table *symbolTable, instruction input) {
  // TODO: (WIP) I refactored your `remBracket` & added var `addresses` - Alex
  word *addresses = remBracket(input.fields[1]);

  // decoding address type
  SDTIOperation operation = SDTIparser(input.fields, input.field_count);
  // Load bit
  word l = (!strcmp("ldr", input.opcode)) ? (1 << SDTI_L_SHIFT) : 0;
  // PRE/POST-INDEXING bits
  word p = (operation == POST_RN_EXP) ? 0 : (1 << SDTI_P_SHIFT);
  // base register Rn
  word Rn = addresses[0] << SDTI_RN_SHIFT;
  // source/ dest register Rd
  word Rd = rem(input.fields[0]) << SDTI_RD_SHIFT;
  // offsets
  word offset;
  // switch case for different address types
  switch (operation) {
  case POST_RN_EXP:
    // offset
    offset = rem(input.fields[2]);
  case PRE_RN:
    // Offset is 0
    offset = 0;
  case PRE_RN_EXP:
    // offset
    // TODO: check if this is null? - Alex
    offset = addresses[1];
  case NUMERIC_CONST:
    // check if expression can fit inside a mov function
    if (rem(input.fields[1]) <= SDTI_EXP_BOUND) {
      return assembleDPI(symbolTable, input);
    } else {
      // offset
      offset = getSymbol(symbolTable, input.fields[1])->body.address;
      // base register Rn
      Rn = 15;
    }
  default:
    // this should never happen, fields were most likely parsed wrong
    assert(false);
  }
  // immediate offsets
  word i = 1 << SDTI_I_SHIFT;
  // up bit
  word u = 0;
  // freeing memory for the register address and expression address
  free(addresses);
  // returning constructed instruction
  return ALWAYS | SDTI_HARDCODE | i | p | u | l | Rn | Rd | offset;
}
