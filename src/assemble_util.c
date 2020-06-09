#include "assemble_util.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_FILE_SIZE 16

typedef enum SDTIOperation {
  NUMERIC_CONST,
  PRE_RN,
  PRE_RN_EXP,
  POST_RN_EXP
} SDTIOperation;

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

word rem(char *string) {
  char *c = "[";
  if (&string[0] == &c[0]) {
    return atoi(string + 2);
  }
  return atoi(string++);
}
// this function decodes the address provided within the instruction struct
SDTIOperation SDTIdecode(char **fields, uint field_count) {
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

word assembleSDTI(instruction *input) {
  char *ldr = "ldr";
  // Load bit
  word l;
  (strcmp(input->opcode, ldr) == 0) ? (l = 0 << 31) : (l = 1 << 31);
  // PRE/POST-INDEXING bits
  word p;
  // base register Rn
  word Rn;
  // offsets
  word offset;
  // source/ dest register Rd
  word Rd = rem(input->fields[0]) << SDTI_RD_SHIFT;
  switch (SDTIdecode(input->fields, input->field_count)) {
  case POST_RN_EXP:
    // setting post indexing bit
    p = 0 << SDTI_P_SHIFT;
    // base register Rn
    Rn = rem(input->fields[1]) << SDTI_RN_SHIFT;
    // offset
    offset = rem(input->fields[2]);
  }
  // immediate offsets
  word i = 1 << SDTI_I_SHIFT;
  // up bits
  word u = 1 << SDTI_U_SHIFT;

  word always = 1 << 31;
  word set = 1 << 26;
  return always | set | i | p | u | l | Rn | Rd | offset;
}
/*

int main(void) {
  instruction *input = malloc(sizeof(instruction));
  input->opcode = "ldr";
  input->fields = malloc(sizeof(input->fields));
  input->fields[0] = "[r3]";
  input->fields[1] = "0x10";
  input->field_count = 3;
  word output = assembleSDTI(input);
  printBits(output);
  free(input->fields);
  free(input);
  return 1;
}
*/
