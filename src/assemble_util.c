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

int lookup(pair_t table[], const char *key) {
  // TODO: determine TABLE_LENGTH
  for (int i = 0; i < TABLE_LENGTH; i++) {
    if (!strcmp(table[i]->key, key)) {
      return table[i]->value;
    }
  }
  // TODO: handle error for no (key, value) pair in table
  return NULL;
}

enum Opcode parseDPIOpcode(char *mnemonic) {
  return lookup(opcodeTable, mnemonic) << DPI_OPCODE_SHIFT;
}

uint8_t parseImmediate(const char *op2) {
  uint8_t imm;
  // TODO: check length before
  if (op2[1] == '0' && op2[2] == 'x') {
    // TODO: parse hex value
  } else {
    imm = rem(op2);
  }
  return imm;
}

enum Shift parseShiftType(char *shift) { return lookup(shiftTable, shift); }

word parseOperand2(const char **op2) {
  word operand2;
  // <#expression> is a numeric constant - an 8 bit immediate value
  // decimal or hexadecimal (“#0x...”)
  if (IS_IMMEDIATE(op2[0])) {
    // TODO: throw error if numeric constant cannot be represented
    // TODO: determine 4-bit for rotation number
    uint8_t imm = parseImmediate(op2[0]);
    return operand2;
  }

  // shifted register - Rm <shiftname> {<register> or <#expression>}

  uint rm = rem(op2[0]);
  enum Shift shiftType = parseShiftType(op2[1]);
  if (IS_IMMEDIATE(op2[2])) {
    uint shiftNum = parseImmediate(op2[2]);
    return (shiftNum << 3) & (shiftType << 1);
  }

  uint rs = rem(op2[2]);
  // Rs can be any general purpose register except the PC
  assert(rs != PC);
  return (rs << 4) & (shiftType << 1) & 1;
}

word assembleDPI(symbol_table *symbolTable, instruction *input) {
  // TODO: parse instruction
  // TODO: generate 32 bit word
  // TODO: figure out if symbolTable is needed

  word i;
  word opcode = parseDPIOpcode(input->opcode);
  word s = 0;
  word rn = 0;
  word rd = 0;
  word op2;

  // instruction: mov
  // syntax: mov Rd, <Operand2>
  if (opcode == MOV) {
    i = IS_IMMEDIATE(input->fields[1]) ? 1 << DPI_I_SHIFT : 0;
    rd = rem(input->fields[0]) << DPI_RD_SHIFT;
    op2 = parseOperand2(input->fields + 1);
  }

  // instructions: tst, teq, cmp
  // syntax: <opcode> Rn, <Operand2>

  else if (opcode == TST || opcode == TEQ || opcode == CMP) {
    i = IS_IMMEDIATE(input->fields[1]) ? 1 << DPI_I_SHIFT : 0;
    s = 1 << INSTRUCTION_S_MASK;
    rn = rem(input->fields[0]) << DPI_RN_SHIFT;
    op2 = parseOperand2(input->fields + 1);
  }

  // instructions: and, eor, sub, rsb, add, orr
  // syntax: <opcode> Rd, Rn, <Operand2>
  else {
    i = IS_IMMEDIATE(input->fields[2]) ? 1 << DPI_I_SHIFT : 0;
    rn = rem(input->fields[1]) << DPI_RN_SHIFT;
    rd = rem(input->fields[0]) << DPI_RD_SHIFT;
    op2 = parseOperand2(input->fields + 2);
  }

  // TODO: (WIP) remove code redundancy
  // i = IS_IMMEDIATE() ? DPI_I_SHIFT;
  // op2 = parseOperand2();
  return DPI_COND | i | opcode | s | rn | rd | op2;
}
