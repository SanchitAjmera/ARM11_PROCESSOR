#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "include/file_lines.h"
#include "include/symbol_table.h"
#include <stdio.h>

extern file_lines *scanFile(FILE *armFile, symbol_table *symbolTable);

// used for assemble dpi
#define IS_IMMEDIATE(op) (op[0] == '#')
#define DPI_COND (14 << 28) // 1110 (al)
// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// shift types
enum Shift { LSL, LSR, ASR, ROR };
// generic (key, value) struct for lookups
typedef struct {
  char *key;
  int value;
} pair_t;
// TODO: (WIP) lookup tables (static and const?)
pair_t shiftTable[] = {{"LSL", LSL}, {"LSR", LSR}, {"ASR", ASR}, {"ROR", ROR}};

pair_t opcodeTable[] = {{"AND", AND}, {"EOR", EOR}, {"SUB", SUB}, {"RSB", RSB},
                        {"ADD", ADD}, {"TST", TST}, {"TEQ", TEQ}, {"CMP", CMP},
                        {"ORR", ORR}, {"MOV", MOV}};
#endif
