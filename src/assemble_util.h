#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "symbol_table.h"
#include <stdio.h>

typedef struct {
  char **lines;
  uint fileLinesCount;
} file_lines;

typedef struct {
  char *key;
  int value;
} pair_t;

extern file_lines *scanFile(FILE *armFile, symbol_table *symbolTable);

// Will be removed from this file in future (accessible from constants)
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };

// To be used with Lookup for string to enum conversion in getCondition;
static const pair_t condTable[] = {{"eq", EQ}, {"ne", NE}, {"ge", GE},
                                   {"lt", LT}, {"gt", GT}, {"le", LE},
                                   {"al", AL}};

#endif
