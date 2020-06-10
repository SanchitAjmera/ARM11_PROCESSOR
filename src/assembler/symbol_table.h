#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../common/constants.h"
#include <stdbool.h>
#include <stdint.h>

struct instruction;
struct symbol_table;
struct symbol;

typedef struct instruction instruction;
typedef struct symbol_table symbol_table;
typedef struct symbol symbol;

enum SymbolType { LABEL, INSTR };

struct instruction {
  char *opcode;  // String representing the instruction INCLUDING cond suffix
  char **fields; // Array of string fields.
  uint field_count;
  word currentAddress; // The current address of the instruction being called
};

struct symbol_table {
  symbol *symbols;  // currently implemented as array of symbols
  uint symbolCount; // number of entries in the symbol table
  uint maxSymbols;  // size of the array storing the symbol table
};

struct symbol {
  char *name;
  enum SymbolType type;
  union {
    word address;                                      // LABEL
    word (*assembleFunc)(symbol_table *, instruction); // INSTR
  } body;
  /* TODO: make symbol_table an abstract binary search tree?
  symbol *left, *right; */
};

extern symbol_table *newSymbolTable();
extern symbol *getSymbol(const symbol_table *s, const char *name);
extern void addSymbol(symbol_table *symbolTable, symbol entry);
extern void freeTable(symbol_table *symbolTable);
extern void printSymbolTable(symbol_table *s);

#endif
