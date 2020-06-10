#include "assemble_util.h"
#include "include/constants.h"
#include "include/file_lines.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_CHAR_LIM 512
#define WORD_SIZE_BYTES 4

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

// removes first character and returns integer from string
word rem(char *string) { return atoi(++string); }

int lookup(const pair_t table[], int size, const char *key) {
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i]->key, key)) {
      return table[i]->value;
    }
  }
  return LOOKUP_FAILURE;
}

/*Provides assembly function for 'mla' and 'mul' instructions and
returns the corresponding ARM-binary based on the instruction arguments*/
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

/* Converts all intructions related to a branch instruction, including
both conditional and unconditional, to the corresponding ARM-binary */
word assembleBranch(symbol_table *symbolTable, instruction *input) {
  // The 'b' instruction is always executed
  // Otherwise the condition of the branch will be the letters following 'b'
  word cond = !strcmp(input->opcode, "b")
                  ? ALWAYS
                  : lookup(condTable, ++(input->opcode)) << COND_SHIFT;

  word currentAddress = input->currentAddress;
  char *target = input->fields[0];

  // A hashtag denotes a constant address, otherwise the address needs
  // found from the symbol table
  word targetAddress = (target[0] == '#')
                           ? rem(target)
                           : getSymbol(symbolTable, target)->body.address;
  // Calculates the offset with the pipeline effect considered
  word offset = (targetAddress - currentAddress + 8) >> 2;

  return cond | BRANCH_HARDCODE | (offset | BRANCH_OFFSET_MASK);

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

  word assembleSDTI(symbol_table * symbolTable, instruction * input) {
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
      // check if expression can fit inside a mov function
      if (rem(input->fields[1]) <= SDTI_EXP_BOUND) {
        return assembleDPI(symbolTable, input);
      } else {
        // offset
        offset = getSymbol(symbolTable, input->fields[1])->body.address;
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
    // returning constructed instruction
    return ALWAYS | SDTI_HARDCODE | i | p | u | l | Rn | Rd | offset;
  }
