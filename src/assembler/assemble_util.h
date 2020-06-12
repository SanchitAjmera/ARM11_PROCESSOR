#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "../common/util.h"
#include "file_lines.h"
#include "symbol_table.h"
#include <stdio.h>

// Generic (string, enum) struct for lookups
typedef struct {
  char *key;
  word value;
} pair_t;

// Enum for different types of addresses in SDTI assembly instructions
typedef enum { NUMERIC_CONST, PRE_RN, PRE_RN_EXP, POST_RN_EXP } SDTIOperation;

// Tables to be used with Lookup for string to enum conversions
static const pair_t condTable[] = {{"eq", EQ}, {"ne", NE}, {"ge", GE},
                                   {"lt", LT}, {"gt", GT}, {"le", LE},
                                   {"al", AL}};
static const pair_t shiftTable[] = {
    {"lsl", LSL}, {"lsr", LSR}, {"asr", ASR}, {"ror", ROR}};

static const pair_t opcodeTable[] = {
    {"and", AND}, {"eor", EOR}, {"sub", SUB},     {"rsb", RSB},
    {"add", ADD}, {"tst", TST}, {"teq", TEQ},     {"cmp", CMP},
    {"orr", ORR}, {"mov", MOV}, {"andeq", ANDEQ}, {"lsl", LSL_SPECIAL}};

// Assemble functions for all instruction types
extern word assembleDPI(symbol_table *symbolTable, instruction input);
extern word assembleSDTI(symbol_table *symbolTable, instruction input);
extern word assembleMultiply(symbol_table *symbolTable, instruction input);
extern word assembleBranch(symbol_table *symbolTable, instruction input);
extern uint parseImmediate(char *op2);

extern void scanFile(FILE *armFile, symbol_table *symbolTable,
                     file_lines *output);
extern void parseLines(file_lines *in, symbol_table *symbolTable, FILE *out);

#endif
