#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "../../common/constants.h"
#include "../../common/util.h"
#include "../assemble_constants.h"
#include "../file_lines/file_lines.h"
#include "../symbol_table/symbol_table.h"
#include "assemble_util.h"

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
    {"orr", ORR}, {"mov", MOV}, {"andeq", ANDEQ}, {"lsl", LSL_SPECIAL},
    {"ldr", LDR}, {"str", STR}, {"mul", MUL},     {"mla", MLA},
    {"beq", BEQ}, {"bne", BNE}, {"bge", BGE},     {"blt", BLT},
    {"bgt", BGT}, {"ble", BLE}, {"b", B}};

// Assemble functions for all instruction types
extern word assembleSDTI(symbol_table *symbolTable, instruction input);
extern word assembleMultiply(symbol_table *symbolTable, instruction input);
extern word assembleBranch(symbol_table *symbolTable, instruction input);
extern uint parseImmediate(char *op2);
extern void scanFile(FILE *armFile, symbol_table *symbolTable,
                     fileLines_t *output);
extern void parseLines(fileLines_t *in, symbol_table *symbolTable, FILE *out);
extern word parseLine(symbol_table *symbolTable, const char *line,
                      word address);
extern int lookup(const pair_t table[], const int size, const char *key);

#endif
