#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "symbol_table.h"
#include <stdio.h>

/* Structure that stores the ARM file's lines as an array of strings */
typedef struct {
  char **lines;
  uint lineCount, maxLines;
} file_lines;

extern file_lines *scanFile(FILE *armFile, symbol_table *symbolTable);
extern void freeFileLines(file_lines *fileLines);
extern void printFileLines(file_lines *fileLines);

#endif
