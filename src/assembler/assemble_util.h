#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "../common/util.h"
#include "file_lines.h"
#include "symbol_table.h"
#include <stdio.h>

#define LINE_CHAR_LIM 512
#define WORD_SIZE_BYTES 4

// generic (string, enum) struct for lookups
typedef struct {
  char *key;
  int value;
} pair_t;

// Enum for different types of addresses in SDTI assembly instructions
typedef enum SDTIOperation {
  NUMERIC_CONST,
  PRE_RN,
  PRE_RN_EXP,
  POST_RN_EXP
} SDTIOperation;

// Will be removed from this file in future (accessible from constants)
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };

// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// shift types
enum Shift { LSL, LSR, ASR, ROR };

// TODO: (WIP) lookup tables
// To be used with Lookup for string to enum conversion in getCondition;
static const pair_t condTable[] = {{"eq", EQ}, {"ne", NE}, {"ge", GE},
                                   {"lt", LT}, {"gt", GT}, {"le", LE},
                                   {"al", AL}};
static const pair_t shiftTable[] = {
    {"LSL", LSL}, {"LSR", LSR}, {"ASR", ASR}, {"ROR", ROR}};

static const pair_t opcodeTable[] = {
    {"AND", AND}, {"EOR", EOR}, {"SUB", SUB}, {"RSB", RSB}, {"ADD", ADD},
    {"TST", TST}, {"TEQ", TEQ}, {"CMP", CMP}, {"ORR", ORR}, {"MOV", MOV}};

extern word assembleDPI(symbol_table *symbolTable, instruction input);
extern word assembleSDTI(symbol_table *symbolTable, instruction input);
extern word assembleMultiply(symbol_table *symbolTable, instruction input);
extern word assembleBranch(symbol_table *symbolTable, instruction input);

extern void scanFile(FILE *armFile, symbol_table *symbolTable,
                     file_lines *output);
extern void parseLines(file_lines *in, symbol_table *symbolTable, FILE *out);

#endif
