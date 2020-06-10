#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "include/file_lines.h"
#include "include/symbol_table.h"
#include <stdio.h>

extern file_lines *scanFile(FILE *armFile, symbol_table *symbolTable);

extern word assembleSDTI(symbol_table *symbolTable, instruction *input);

#endif
