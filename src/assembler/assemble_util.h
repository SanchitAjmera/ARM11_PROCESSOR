#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "../common/util.h"
#include "file_lines.h"
#include "symbol_table.h"
#include <stdio.h>

#define LINE_CHAR_LIM 512
#define WORD_SIZE_BYTES 4
#define ROTATION_FACTOR 2

/*registers 0-12 will be used by their value so for reg0 we can just use 0
but these will make it easier to address in memory*/
enum Register { PC = 15, CPSR = 16 };
// opcode mnemonics
enum Opcode {
  AND,
  EOR,
  SUB,
  RSB,
  ADD,
  TST = 8,
  TEQ,
  CMP,
  ORR = 12,
  MOV,
  ANDEQ,
  LSL,
};
// condition suffixes
enum Cond { EQ, NE, GE = 10, LT, GT, LE, AL };
// shift types
enum Shift { LSL, LSR, ASR, ROR };

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

// To be used with Lookup for string to enum conversion in getCondition;
static const pair_t condTable[] = {{"eq", EQ}, {"ne", NE}, {"ge", GE},
                                   {"lt", LT}, {"gt", GT}, {"le", LE},
                                   {"al", AL}};
static const pair_t shiftTable[] = {
    {"lsl", LSL}, {"lsr", LSR}, {"asr", ASR}, {"ror", ROR}};

static const pair_t opcodeTable[] = {
    {"and", AND}, {"eor", EOR}, {"sub", SUB},     {"rsb", RSB},
    {"add", ADD}, {"tst", TST}, {"teq", TEQ},     {"cmp", CMP},
    {"orr", ORR}, {"mov", MOV}, {"andeq", ANDEQ}, {"lsl", LSL}};

extern word assembleDPI(symbol_table *symbolTable, instruction input);
extern word assembleSDTI(symbol_table *symbolTable, instruction input);
extern word assembleMultiply(symbol_table *symbolTable, instruction input);
extern word assembleBranch(symbol_table *symbolTable, instruction input);
extern uint parseImmediate(char *op2);

extern void scanFile(FILE *armFile, symbol_table *symbolTable,
                     file_lines *output);
extern void parseLines(file_lines *in, symbol_table *symbolTable, FILE *out);

#endif
