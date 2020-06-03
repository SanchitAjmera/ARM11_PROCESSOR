#include "assemble_util.h"
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

word rem(char *string) { return atoi(++string); }

word assembleSDTI(instruction *input) {}
