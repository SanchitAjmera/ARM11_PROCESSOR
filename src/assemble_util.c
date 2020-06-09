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

// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };

enum Opcode parseDPIOpcode(char *mnemonic) {
  // TODO: fix parse on mnemonic
  switch (mnemonic) {
  case "and":
    return AND;
  case "eor":
    return EOR;
  case "sub":
    return SUB;
  case "rsb":
    return RSB;
  case "add":
    return ADD;
  case "tst":
    return TST;
  case "teq":
    return TEQ;
  case "cmp":
    return CMP;
  case "orr":
    return ORR;
  case "mov":
    return MOV;
  default:
    // no other case
    // should never happen
    assert(false);
  }
}

// shift types
enum Shift { LSL, LSR, ASR, ROR };

uint parseOperand2(char *op2) {
  // <#expression> is a numeric constant - an 8 bit immediate value
  // decimal or hexadecimal (“#0x...”)
  if (op2[0] == '#') {
    // TODO: throw error if numeric constant cannot be represented
    // TODO: determine 4-bit for rotation number
    uint8_t imm;
    if (op2[1] == '0' && op2[2] == 'x') {
      imm = atoi(op2 + 2);
    }
  } else {
    imm = rem(op2);
    return imm;
  }

  // shifted register, Rm{,<shift>}
  // <shift> {<shiftname> <register> or <shiftname> <#expression>}
  // <shiftname> {enum Shift}
}

#define DPI_COND (14) // 1110

word assembleDPI(symbol_table *symbolTable, instruction *input) {
  // TODO: parse instruction
  // TODO: generate 32 bit word
  // TODO: figure out if symbolTable needed

  // instructions: and, eor, sub, rsb, add, orr
  // syntax: <opcode> Rd, Rn, <Operand2>
  if (input->field_count == 4) {
  }

  // instruction: mov
  // syntax: mov Rd, <Operand2>
  if (input->opcde == "mov") {
    uint i;
    uint opcode = parseDPIOpcode(input->opcode);
    uint s = 0;
    uint rn = ;
    uint rd = rem(input->fields[0]);
    uint op2 = parseOperand2(input->fields[1]);
  }

  // instructions: tst, teq, cmp
  // syntax: <opcode> Rn, <Operand2>

  return 0x0;
}
