#include "symbol_table.h"
#include "../common/util.h"
#include "assemble_constants.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR (0.75)

symbol **createSymbols(int num, int size) {
  symbol **symbols = malloc(num * size);
  validatePtr(symbols, "Not enough memory.");
  for (int i = 0; i < num; i++) {
    symbols[i] = malloc(sizeof(*symbols[i]));
    validatePtr(symbols[i], "Not enough memory.");
    symbols[i][0].collisions = 0;
  }
  return symbols;
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

void freeList(symbol *symbols, int size) {
  for (int i = 0; i < size; i++) {
    free(symbols[i].name);
  }
  free(symbols);
}

void freeSymbols(symbol **symbols, int size) {
  for (int i = 0; i < size; i++) {
    freeList(symbols[i], symbols[i][0].collisions);
  }
  free(symbols);
}

void freeTable(symbol_table *s) {
  freeSymbols(s->symbols, s->size);
  free(s);
}

static int hash(const symbol_table *s, const char *key) {
  int index = 7;
  for (int i = 0; i < strlen(key); i++) {
    index = (index * 31) + key[i];
  }
  return index % s->size;
}

symbol *getSymbol(const symbol_table *s, const char *name) {
  int index1 = hash(s, name);
  int size = s->symbols[index1][0].collisions;
  if (size >= 1) {
    for (int i = 0; i < size; i++) {
      if (!strcmp(s->symbols[index1][i].name, name)) {
        return s->symbols[index1] + i;
      }
    }
  }
  return s->symbols[index1];
}

void addSymbols(symbol_table *s, symbol **symbols, int symbolCount) {
  for (int i = 0; i < symbolCount; i++) {
    addSymbol(s, symbols[i]);
  }
}

static void rehash(symbol_table *s) {
  if (s->symbolCount / s->size < LOAD_FACTOR) {
    return;
  }
  symbol **symbols = createSymbols(s->symbolCount, sizeof(*symbols));
  int index = 0;
  symbol temp = {NULL, LABEL, 0, .body.assembleFunc = NULL};
  for (int i = 0; i < s->size; i++) {
    for (int j = 0; j < s->symbols[i][0].collisions; j++) {
      symbols[index++][0] = s->symbols[i][j];
      s->symbols[i][j] = temp;
    }
  }
  // freeSymbols(s->symbols, s->size);
  s->size *= 2;
  s->symbols = realloc(s->symbols, sizeof(*s->symbols) * s->size);
  addSymbols(s, symbols, s->symbolCount);
  // freeSymbols(symbols, s->symbolCount);
  free(symbols);
}

void addSymbol(symbol_table *s, symbol *entry) {
  if (getSymbol(s, entry->name)->name != NULL) {
    if (!strcmp(getSymbol(s, entry->name)->name, entry->name)) {
      return;
    }
    // label already defined
  }

  int index1 = hash(s, entry->name);
  int index2 = 0;
  int size = s->symbols[index1][0].collisions;
  if (size >= 1) {
    s->symbols[index1] =
        realloc(s->symbols[index1], sizeof(*s->symbols[index1]) * (++size));
    validatePtr(s->symbols[index1], "Not enough memory.");
    index2 = size - 1;
  }
  s->symbols[index1][index2] = *entry;
  s->symbolCount++;
  s->symbols[index1][0].collisions++;
  rehash(s);
}

// dummy functions for compilation

void printSymbol(symbol s) {}

void printSymbolTable(symbol_table *s) {
  for (int i = 0; i < s->size; i++) {
    if (s->symbols[i] == NULL) {
      continue;
    }
    for (int j = 0; j < s->symbols[i][0].collisions; j++) {
      printf("key:%s\n", s->symbols[i][j].name);
    }
  }
}
