#include "assemble_util.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_FILE_SIZE 16

/* Scans a file adding labels to the symbol table,
    as well as expressions. Returns an array of strings that
    represent each line, stripped of the newline \n,
    and stores expressions in their string form at the end of the array. */
file_lines *scanFile(FILE *armFile, symbol_table *symbolTable) {
  int linesRead = 0;
  // int expresssionsRead = 0;

  file_lines *fileLines = malloc(sizeof(fileLines));

  char line[512];
  while (fgets(line, 512, armFile) != NULL) {
    linesRead++;
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == ':') { // Line is a label
        /* TODO: extract label, calculate address, and store in symbol table */
      } else if (line[i] == '#') { // Line contains an expression
        char *expression =
            strtok(line + i - 1, " ],\n"); // Pulls expression out
        /* TODO: add expression to a list of expressions that need to be
          added to the end of the file. Their address offsets can be
          calculated once we determine the total number of lines in armFile */
        printf("%s\n", expression);
      }
    }
  }

  return fileLines;
}

// removes first character and returns integer from string
word rem(char *string) { return atoi(++string); }

word assembleMultiply(symbol_table *symbolTable, instruction *input) {
  // Defining the components of the instruction
  word rd = rem(input->fields[0]) << MULT_RDEST_SHIFT;
  word rm = rem(input->fields[1]);
  word rs = rem(input->fields[2]) << MULT_REG_S_SHIFT;

  // Initialising for 'mul', may be updated if 'mla'
  word rn = 0;
  word accumulate = 0;

  // set rn and A for an 'accumulate' input
  if (!strcmp(input->opcode, "mla")) {
    rn = rem(input->fields[3]) << MULT_REG_N_SHIFT;
    accumulate = ACCUMULATE_FLAG;
  }
  // S is set to 0 so no need to explicitly write it
  // Bits 4-7 are hardcoded as 1001
  return ALWAYS | accumulate | rd | rn | rs | MULT_HARDCODE | rm;
}

// this is from emulate_util.h so could we maybe put it in constants.h
// a lot of reused parts and #including emulate_util is not nice
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };

// No function in C which converts string to enum so
// had to use this
word getCondition(const char *condition) {
  if (!strcmp(condition, "eq")) {
    return EQ;
  }
  if (!strcmp(condition, "ne")) {
    return NE;
  }
  if (!strcmp(condition, "ge")) {
    return GE;
  }
  if (!strcmp(condition, "lt")) {
    return LT;
  }
  if (!strcmp(condition, "gt")) {
    return GT;
  }
  if (!strcmp(condition, "le")) {
    return LE;
  }
  return ALWAYS;
}
// to complete
word assembleBranch(symbol_table *symbolTable, instruction *input) {
  // need to change condition into an enum to cover all conds
  word cond =
      strlen(input->opcode) == 1 ? ALWAYS : getCondition(++(input->opcode));

  // need to find out how to obtain current address
  word currentAddress = 0;

  char *target = input->fields[0];
  word targetAddress = (target[0] == '#')
                           ? rem(target)
                           : getSymbol(symbolTable, target)->body.address;
  word offset = (targetAddress - currentAddress + 8) >> 2;

  return cond | BRANCH_HARDCODE | (offset | BRANCH_OFFSET_MASK);
}
