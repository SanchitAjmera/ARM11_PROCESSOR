#include "assemble_util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_FILE_SIZE 16
#define LINE_CHAR_LIM 512

/* TODO: move these functions to a new file for the fileLines structure */
#define INIT_FILE_LINES_SIZE 16
void addLine(file_lines *fl, char *line) {
  printf("new line: %s\n", line);
  if (fl->lineCount == fl->maxLines) {
    fl->lines = realloc(fl->lines, fl->maxLines *= 2);
  }
  // Have to allocate memory for each string
  fl->lines[fl->lineCount] = malloc(sizeof(char) * (strlen(line) + 1));
  strncpy(fl->lines[fl->lineCount], line, strlen(line));
  fl->lineCount++;
}

void addLines(file_lines *fl, char **lines, uint n) {
  for (int i = 0; i < n; i++) {
    addLine(fl, lines[i]);
  }
}

void printFileLines(file_lines *fileLines) {
  for (int i = 0; i < fileLines->lineCount; i++) {
    printf("%i: %s\n", i, fileLines->lines[i]);
  }
}

void initFileLines(file_lines *fileLines) {
  assert(fileLines != NULL);
  fileLines->maxLines = INIT_FILE_LINES_SIZE;
  fileLines->lineCount = 0;
  fileLines->lines = malloc(sizeof(char *) * fileLines->maxLines);
  assert(fileLines->lines != NULL);
}

void freeFileLines(file_lines *fileLines) {
  for (int i = 0; i < fileLines->lineCount; i++) {
    free(fileLines->lines[i]);
  }
  free(fileLines->lines);
  free(fileLines);
}

/* Scans a file adding labels to the symbol table,
    as well as expressions. Returns an array of strings that
    represent each line, stripped of the newline \n,
    and stores expressions in their string form at the end of the array. */
file_lines *scanFile(FILE *armFile, symbol_table *symbolTable) {
  assert(armFile != NULL && symbolTable != NULL);
  int linesRead = 0;

  /* Build a new file_lines struct which stores each line in the
    ARM file in an array, and any expressions gathered at the end */
  file_lines *fileLines = malloc(sizeof(file_lines));
  initFileLines(fileLines);

  /* Stores expressions found during the scan */
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
        symbol labelSymbol = {NULL, LABEL, .body.address = linesRead * 4};
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
    linesRead++;
  }

  /* Now that we have determined the number of lines in the ARM file,
     we can add all expressions to the symbol table */
  for (int i = 0; i < expressions->lineCount; i++) {
    char *expr = expressions->lines[i];
    // Make a new symbol for this expression
    word address = (linesRead + i) * 4;
    symbol exprSymbol = {NULL, LABEL, .body.address = address};
    exprSymbol.name = malloc(sizeof(char) * strlen(expr));
    strcpy(exprSymbol.name, expr);

    addSymbol(symbolTable, exprSymbol);
  }

  addLines(fileLines, expressions->lines, expressions->lineCount);

  freeFileLines(expressions);
  return fileLines;
}
