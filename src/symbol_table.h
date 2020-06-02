#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct {
  Opcode opcode;
  /*
   Stores 2 pointers - one to the string tokens of the parts of the input that
   represent registers, and the other to the tokens that compose operand2.
  */
  char **registers, **operand2;
  uint regCount, operandCount;
  /*
  We can add a union of structs that represent dpi, sdri, branch, mult later
  if this makes it easier to code
  */
} instruction;

typedef struct {
  char *name;
  bool isLabel;
  union {
    word address; // used if isLabel == true
    struct {      // used if isLabel == false
      word (*assembleFunc)(symbol_table, instruction);
      uint operandCount;
    }
  };
} symbol;

typedef struct {
  symbol **symbols; // An index on the heap of pointers to symbols
  uint entry_count;
  uint max_entries;
} symbol_table;

extern symbol *getSymbol(symbol_table *symbolTable, char *name);
extern symbol *addSymbol(symbol_table *symbolTable, symbol entry);
extern void freeTable(symbol_table *symbolTable);

#endif
