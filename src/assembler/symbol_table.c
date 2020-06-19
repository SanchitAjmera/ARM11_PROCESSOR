#include "symbol_table.h"
#include "../common/util.h"
#include "assemble_constants.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

symbol *newSymbol(void) {
  symbol *s = malloc(sizeof(symbol));
  s->name = NULL;
  return s;
}

symbol **createSymbols(int num, int size) {
  symbol **symbols = malloc(num * size);
  validatePtr(symbols, MEM_ASSIGN);
  for (int i = 0; i < num; i++) {
    symbols[i] = newSymbol();
    validatePtr(symbols[i], MEM_ASSIGN);
    symbols[i][0].collisions = 0;
  }
  return symbols;
}

/* Takes in a pointer to an uninitialised symbol table and initialises it */
symbol_table *newSymbolTable(void) {
  symbol_table *s = malloc(sizeof(symbol_table));
  validatePtr(s, MEM_ASSIGN);
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
  int index = PRIME_INIT;
  for (const char *copy = key; *copy; copy++) {
    index = (index * PRIME_FACTOR) + copy[0];
  }
  // for (int i = 0; i < strlen(key); i++) {
  //   index = (index * PRIME_FACTOR) + key[i];
  // }
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
  // overwrites the hash table make it 'empty'
  symbol reset = {NULL, LABEL, 0, .body.assembleFunc = NULL};
  for (int i = 0; i < s->size; i++) {
    for (int j = 0; j < s->symbols[i][0].collisions; j++) {
      symbols[index++][0] = s->symbols[i][j];
      s->symbols[i][j] = reset;
    }
    // re sizing the collision list back to 1
    s->symbols[i] = realloc(s->symbols[i], sizeof(*s->symbols));
    validatePtr(s->symbols[i], MEM_ASSIGN);
  }
  s->size *= 2;
  s->symbols = realloc(s->symbols, sizeof(*s->symbols) * s->size);
  validatePtr(s->symbols, MEM_ASSIGN);
  addSymbols(s, symbols, s->symbolCount);
  free(symbols);
}

void addSymbol(symbol_table *s, symbol *entry) {
  symbol *check = getSymbol(s, entry->name);
  if (check->name != NULL && (strcmp(check->name, entry->name) == 0)) {
    // label already defined
    return;
  }
  int index1 = hash(s, entry->name);
  int size = s->symbols[index1][0].collisions;
  if (size >= 1) {
    s->symbols[index1] =
        realloc(s->symbols[index1], sizeof(symbol) * (size + 1));
    validatePtr(s->symbols[index1], MEM_ASSIGN);
  }
  s->symbols[index1][size] = *entry;
  s->symbolCount++;
  s->symbols[index1][0].collisions++;
  rehash(s);
}

// TODO: delete functions

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
