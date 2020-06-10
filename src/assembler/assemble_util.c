#include "assemble_util.h"
#include "file_lines.h"
#include "../common/constants.h"
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
void scanFile(FILE *armFile, symbol_table *symbolTable, file_lines *output) {
  assert(armFile != NULL && symbolTable != NULL && output != NULL);

  /* Will be used to store expressions found during the scan */
  file_lines *expressions = newFileLines();

  /* Scan file for labels and expressions */
  char line[LINE_CHAR_LIM];
  while (fgets(line, LINE_CHAR_LIM, armFile) != NULL) {
    addLine(output, strtok(line, "\n")); // Adds line stripped of \n

    /* iterate through chars in line */
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == ':') { // Line is a label
        char *label = strtok(line, "\n");
        // Make a new symbol for this label
        symbol labelSymbol = {NULL, LABEL,
                              .body.address =
                                  output->lineCount * WORD_SIZE_BYTES};
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

  /* Now that we have added all lines to output, we can add all expressions
     to the symbol table and calculate their address using lineCount */
  for (int i = 0; i < expressions->lineCount; i++) {
    char *expr = expressions->lines[i];
    word address = (output->lineCount + i) * WORD_SIZE_BYTES;
    // Make a new symbol for this expression
    symbol exprSymbol = {NULL, LABEL, .body.address = address};
    exprSymbol.name = malloc(sizeof(char) * strlen(expr));
    strcpy(exprSymbol.name, expr);

    addSymbol(symbolTable, exprSymbol);
  }

  addLines(output, expressions->lines, expressions->lineCount);

  freeFileLines(expressions);
}
