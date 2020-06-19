#include "assemble_util.h"
#include "../../common/constants.h"
#include "../../common/util.h"
#include "../assemble_constants.h"
#include "../file_lines.h"
#include "../symbol_table.h"
#include "assemble_dpi.h"
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

/* Performs the first scan on a file adding labels to the symbol table,
    as well as expressions. Returns an array of strings that
    represent each line, stripped of the newline \n,
    and stores expressions in their string form at the end of the array. */
void scanFile(FILE *armFile, symbol_table *symbolTable, fileLines_t *output) {
  // PRE: armFile, symbolTable, output are not NULL
  // Will be used to store expressions found during the scan
  fileLines_t *expressions = newFileLines();
  /* Scan file for labels and expressions */
  char *line = malloc(LINE_CHAR_LIM * sizeof(*line));
  validatePtr(line, MEM_ASSIGN);
  while (fgets(line, LINE_CHAR_LIM, armFile) != NULL) {
    // Iterate through chars in line
    bool isLabel = false;
    char *lineCopy = strptr(line);
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == ':') { // Line is a label
        char *label = strtok(line, ":");
        symbol labelSymbol = {strptr(label), LABEL, 0,
                              .body.address =
                                  output->lineCount * WORD_SIZE_BYTES};
        addSymbol(symbolTable, &labelSymbol);
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
    symbol exprSymbol = {strptr(expr), LABEL, 0, .body.address = address};
    addSymbol(symbolTable, &exprSymbol);
  }

  // Add expressions to the end of the file
  addLines(output, expressions->lines, expressions->lineCount);
  freeFileLines(expressions);
}

/* Performs the second pass on fileLines_t */
void parseLines(fileLines_t *in, symbol_table *symbolTable, FILE *out) {
  // PRE: in, symbolTable, out are not NULL
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
      if (rest[0] == '[') {
        // If the next token starts with a [
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
    validatePtr(instrSymbol, MEM_ASSIGN);
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
static word *remBracket(const char *string) {
  word *addresses = malloc(sizeof(*addresses) * 4);
  validatePtr(addresses, MEM_ASSIGN);
  char *unbracketed = strptr(string + 1);
  // separator
  char *delim = ", ";
  // Gets Rn
  char *token = strtok(unbracketed, delim);
  // gets address of register rn
  if (token[0] == 'r') {
    // Rn could either be a register or immediate const
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
  }
  if (strstr(fields[1], ",")) {
    return PRE_RN_EXP;
  }
  if (strstr(fields[1], "r")) {
    return PRE_RN;
  }
  return NUMERIC_CONST;
}

/* Assembly function for all store and load instructions */
word assembleSDTI(symbol_table *symbolTable, instruction input) {
  word *addresses = remBracket(input.fields[1]);
  // Decoding address type
  SDTIOperation operation = SDTIparser(input.fields, input.field_count);
  // Load bit
  word l = (input.mnemonic == LDR) ? (1 << SDTI_L_SHIFT) : 0;
  // PRE/POST-INDEXING bits
  word p = (operation == POST_RN_EXP) ? 0 : (1 << SDTI_P_SHIFT);
  // Base register Rn
  word rn = addresses[0] << SDTI_RN_SHIFT;
  // Source/ dest register Rd
  word rd = REM_INT(input.fields[0]) << SDTI_RD_SHIFT;
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
      rn = PC << SDTI_RN_SHIFT;
    }
    break;
  default:
    // No other case - if so fields were most likely parsed incorrectly
    errorExit(UNEXPECTED_CASE);
  }
  // Freeing memory for the register address and expression address
  free(addresses);
  // Returning constructed instruction
  return ALWAYS | SDTI_HARDCODE | i | p | u | l | rn | rd | offset;
}
