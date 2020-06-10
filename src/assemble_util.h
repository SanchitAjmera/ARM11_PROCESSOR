#ifndef ASSEMBLE_UTIL_H
#define ASSEMBLE_UTIL_H

#include "include/file_lines.h"
#include "include/symbol_table.h"
#include <stdio.h>

extern file_lines *scanFile(FILE *armFile, symbol_table *symbolTable);

// used for assemble dpi
#define IS_IMMEDIATE(op) (op[0] == '#')
// TODO: MAX_NUM (pow(2, WORD_SIZE) - 1)
#define MAX_NUM (4294967295)
#define OVERFLOW(num) (num > MAX_NUM)
#define MAX_BYTE (0xFF)
#define HEX_BASE (16)
#define LOOKUP_FAILURE (-1)
#define LOOKUP_FAILED(num) (num == LOOKUP_FAILURE)
#define DPI_COND (14 << 28) // 1110 (al)
#define DPI_I_SHIFT 25
#define DPI_OPCODE_SHIFT 21
#define DPI_RN_SHIFT 16
#define DPI_RD_SHIFT 12
#define DPI_S_SHIFT 20
// opcode mnemonics
enum Opcode { AND, EOR, SUB, RSB, ADD, TST = 8, TEQ, CMP, ORR = 12, MOV };
// shift types
enum Shift { LSL, LSR, ASR, ROR };
// generic (string, enum) struct for lookups
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
