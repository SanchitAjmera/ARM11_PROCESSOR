#include "assemble_util.h"
#include "include/constants.h"
#include "include/file_lines.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_CHAR_LIM 512
#define WORD_SIZE_BYTES 4

/* Scans a file adding labels to the symbol table,
    as well as expressions. Returns an array of strings that
    represent each line, stripped of the newline \n,
    and stores expressions in their string form at the end of the array. */
file_lines *scanFile(FILE *armFile, symbol_table *symbolTable) {
  assert(armFile != NULL && symbolTable != NULL);

  /* Build a new file_lines struct which stores each line in the
    ARM file in an array, and any expressions gathered at the end */
  file_lines *fileLines = malloc(sizeof(file_lines));
  initFileLines(fileLines);

  /* Will be used to store expressions found during the scan */
  file_lines *expressions = malloc(sizeof(file_lines));
  initFileLines(expressions);

  /* Scan file for labels and expressions */
  char line[LINE_CHAR_LIM];
  while (fgets(line, LINE_CHAR_LIM, armFile) != NULL) {
    addLine(fileLines, strtok(line, "\n")); // Adds line stripped of \n

    /* iterate through chars in line */
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == ':') { // Line is a label
        char *label = strtok(line, "\n");
        // Make a new symbol for this label
        symbol labelSymbol = {NULL, LABEL,
                              .body.address =
                                  fileLines->lineCount * WORD_SIZE_BYTES};
        labelSymbol.name = malloc(sizeof(char) * strlen(label));
        strcpy(labelSymbol.name, label);

        addSymbol(symbolTable, labelSymbol);
        break; // Line contains only one label
        /* TODO: extract label, calculate address, and store in symbol table */
      } else if (line[i] == '#') { // Line contains an expression
        char *expression =
            strtok(line + i - 1, " ],\n"); // Pulls expression out
        addLine(expressions, expression);
        break; // Each line contains up to one expression. Remove if incorrect
      }
    }
  }

  /* Now that we have added all lines to fileLines, we can add all expressions
     to the symbol table and calculate their address using lineCount */
  for (int i = 0; i < expressions->lineCount; i++) {
    char *expr = expressions->lines[i];
    word address = (fileLines->lineCount + i) * WORD_SIZE_BYTES;
    // Make a new symbol for this expression
    symbol exprSymbol = {NULL, LABEL, .body.address = address};
    exprSymbol.name = malloc(sizeof(char) * strlen(expr));
    strcpy(exprSymbol.name, expr);

    addSymbol(symbolTable, exprSymbol);
  }

  addLines(fileLines, expressions->lines, expressions->lineCount);

  freeFileLines(expressions);
  return fileLines;
}

// returns respective int value, -1 for failure
int lookup(const pair_t table[], const char *key) {
  // TODO: determine size
  int size = 0;
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i]->key, key)) {
      return table[i]->value;
    }
  }
  return LOOKUP_FAILURE;
}

enum Opcode parseDPIOpcode(char *mnemonic) {
  return lookup(opcodeTable, mnemonic) << DPI_OPCODE_SHIFT;
}

uint parseImmediate(const char *op2) {
  if (strlen(op2) > 2) {
    if (op2[1] == '0' && op2[2] == 'x') {
      return (uint)strtol(hex, NULL, HEX_BASE);
    }
  }
  // TODO: check for op2 > MAX_NUM here?
  return (uint)rem(op2);
}

enum Shift parseShiftType(char *shift) { return lookup(shiftTable, shift); }

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
  // TODO: rotate imm
  // TODO: check if it can be represented
  // TODO: return rotation | imm
}

word parseOperand2Imm(const char **op2) {
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

word parseOperand2Reg(const char **op2) {
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

word parseOperand2(const char **op2) {
  word operand2;
  // 8 bit immediate value - <#expression>
  // decimal or hexadecimal ("#n" or “#0x...”)
  if (IS_IMMEDIATE(op2[0])) {
    return parseOperand2Imm(op2);
  }
  // shifted register - Rm <shiftname> {<register> or <#expression>}
  return parseOperand2Reg(op2);
}

word assembleDPI(symbol_table *symbolTable, instruction *input) {
  word opcode = parseDPIOpcode(input->opcode);
  word s = 0;
  word rn = 0;
  word rd = 0;
  char **operand2;
  char *imm;

  // instruction: mov
  // syntax: mov Rd, <Operand2>
  if (opcode == MOV) {
    imm = input->fields[1];
    rd = rem(input->fields[0]);
    operand2 = input->fields + 1;
  }

  // instructions: tst, teq, cmp
  // syntax: <opcode> Rn, <Operand2>

  else if (opcode == TST || opcode == TEQ || opcode == CMP) {
    imm = input->fields[1];
    // COND flags should be updated
    s = 1;
    rn = rem(input->fields[0]);
    operand2 = input->fields + 1;
  }

  // instructions: and, eor, sub, rsb, add, orr
  // syntax: <opcode> Rd, Rn, <Operand2>
  else {
    imm = input->fields[2];
    rn = rem(input->fields[1]);
    rd = rem(input->fields[0]);
    operand2 = input->fields + 2;
  }

  s = s << DPI_S_SHIFT;
  rn = rn << DPI_RN_SHIFT;
  rd = rd << DPI_RD_SHIFT;
  i = IS_IMMEDIATE(imm) ? 1 << DPI_I_SHIFT : 0;
  op2 = parseOperand2(operand2);
  return DPI_COND | i | opcode | s | rn | rd | op2;
}
