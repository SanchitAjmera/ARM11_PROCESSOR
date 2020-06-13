#include "symbol_table.h"
#include "../common/util.h"
#include "assemble_constants.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR (0.75)

static symbol **createSymbols(int num, size_t size) {
  s->symbols = malloc(num * size);
  validatePtr(s->symbols, "Not enough memory.");
  for (int i = 0; i < num; i++) {
    s->symbols[i] = malloc(sizeof(*s->symbols[i]));
    validatePtr(s->symbols[i], "Not enough memory.");
  }
}

/* Takes in a pointer to an uninitialised symbol table and initialises it */
symbol_table *newSymbolTable(void) {
  symbol_table *s = malloc(sizeof(*s));
  validatePtr(s, "Not enough memory.");
  s->size = INIT_S_TABLE_SIZE;
  s->symbolCount = 0;
  s->symbols = createSymbols(s->size, sizeof(*s->symbols));
  return s;
}

static int hash(symbol_table *s, const char *key) { return 0; }

static void rehash(symbol_table *s) {
  if (s->symbolCount / s->size < LOAD_FACTOR) {
    return;
  }
  // TODO: resize hash table & rehash all entries
}

symbol *getSymbol(const symbol_table *s, const char *name) {
  return h->symbols[hash(s, name)][0];
}

void addSymbol(symbol_table *s, symbol *entry) {
  if (getSymbol(s, entry.name) != NULL) {
    // Label already defined
    return;
  }
  // TODO: collision code to be implemented
  s->symbols[hash(s, entry->name)][0] = entry;
}

void addSymbols(symbol_table *s, symbol **symbols, int symbolCount) {
  for (int i = 0; i < symbolCount; i++) {
    addSymbol(s, symbols[i]);
  }
}

void freeTable(symbol_table *s) {
  for (int i = 0; i < s->size; i++) {
    if (s->symbols[i] != NULL) {
      free(s->symbols[i].name);
      free(s->symbols[i]);
    }
  }
  free(s->symbols);
  free(s);
}

// dummy functions for compilation

void printSymbol(symbol s) {}

void printSymbolTable(symbol_table *s) {}
