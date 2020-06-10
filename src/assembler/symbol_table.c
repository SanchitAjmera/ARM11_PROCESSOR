#include "symbol_table.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_S_TABLE_SIZE 32

/* Takes in a pointer to an uninitialised symbol table and initialises it */
symbol_table *newSymbolTable() {
  symbol_table *s = malloc(sizeof(symbol_table));
  assert(s != NULL);
  symbol *symbols = malloc(INIT_S_TABLE_SIZE * sizeof(symbol));
  assert(symbols != NULL);
  s->symbols = symbols;
  s->maxSymbols = INIT_S_TABLE_SIZE;
  s->symbolCount = 0;
  return s;
}

symbol *getSymbol(const symbol_table *s, const char *name) {
  assert(s != NULL);

  /* simple linear search by name */
  for (int i = 0; i < s->symbolCount; i++) {
    if (strcmp(s->symbols[i].name, name) == 0) {
      return s->symbols + i;
    }
  }

  return NULL;
}

void addSymbols(symbol_table *s, symbol *symbols, int symbolCount) {
  for (int i = 0; i < symbolCount; i++) {
    addSymbol(s, symbols[i]);
  }
}

void addSymbol(symbol_table *s, symbol entry) {
  assert(s != NULL);
  if (getSymbol(s, entry.name) != NULL) { // Label already defined
    return;
  }

  if (s->symbolCount == s->maxSymbols) { // Resize symbol table
    s->maxSymbols *= 2;
    if ((s->symbols = realloc(s->symbols, s->maxSymbols)) == NULL) {
      printf("error!");
    }
  }

  s->symbols[s->symbolCount] = entry;
  s->symbolCount += 1;
}

void freeTable(symbol_table *s) {
  for (int i = 0; i < s->maxSymbols; i++) {
    free(s->symbols[i].name);
  }
  free(s->symbols);
  free(s);
}

void printSymbol(symbol s) {
  printf("[name: %s, type: %i, ", s.name, s.type);
  switch (s.type) {
  case LABEL:
    printf("addr: %x", s.body.address);
    break;
  default:
    break;
  }
  printf("]\n");
}

void printSymbolTable(symbol_table *s) {
  for (int i = 0; i < s->symbolCount; i++) {
    printSymbol(s->symbols[i]);
  }
}
