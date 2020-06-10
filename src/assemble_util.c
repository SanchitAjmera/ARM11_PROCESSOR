#include "assemble_util.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_FILE_SIZE 16

// dummy function to allow code to compile]
word assembleDPI(symbol_table *symbolTable, instruction *input) { return 0; }

// Enum for different types of addresses in SDTI assembly instructions
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

// removes first character of string
word rem(char *string) { return atoi(++string); }

// removes bracketing around string
// converts remaining strings into unsigned int values
// returns array containing register address and expression address
word *remBracket(char *string) {
  word *addresses;
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

word assembleSDTI(symbol_table *symbolTable, instruction *input) {
  // decoding address type
  SDTIOperation operation = SDTIparser(input->fields, input->field_count);
  // Load bit
  word l = (!strcmp(input->opcode, "ldr")) ? (1 << SDTI_L_SHIFT) : 0;
  // PRE/POST-INDEXING bits
  word p = (operation == POST_RN_EXP) ? 0 : (1 << SDTI_P_SHIFT);
  // base register Rn
  word Rn = remBracket(input->fields[1])[0] << SDTI_RN_SHIFT;
  // source/ dest register Rd
  word Rd = rem(input->fields[0]) << SDTI_RD_SHIFT;
  // offsets
  word offset;
  // switch case for different address types
  switch (operation) {
  case POST_RN_EXP:
    // offset
    offset = rem(input->fields[2]);
  case PRE_RN:
    // Offset is 0
    offset = 0;
  case PRE_RN_EXP:
    // offset
    offset = remBracket(input->fields[1])[1];
  case NUMERIC_CONST:
    if (rem(input->fields[1]) <= SDTI_EXP_BOUND) {
      return assembleDPI(symbolTable, input);
    } else {
    }
  }
  // immediate offsets
  word i = 1 << SDTI_I_SHIFT;
  // up bits
  word u = 0;

  word ALWAYS = 14 << 28;
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
