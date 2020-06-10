#include "assemble_util.h"
#include "../common/constants.h"
#include "../common/util.h"
#include "file_lines.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_CHAR_LIM 512
#define WORD_SIZE_BYTES 4

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
        char *label = strtok(line, "\n");
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

    for (int j = 0; j < fieldCount; j++) {
      printf("token: %s\n", fields[j]);
    }
    printf("\n");
  }
}
