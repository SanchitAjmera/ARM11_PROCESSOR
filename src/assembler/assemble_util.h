#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "file_lines.h"
#include "symbol_table.h"
#include <stdio.h>

extern void scanFile(FILE *armFile, symbol_table *symbolTable, file_lines *output);

#endif
