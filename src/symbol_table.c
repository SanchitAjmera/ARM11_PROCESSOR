#include "symbol_table.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_S_TABLE_SIZE 32

void initSymbolTable(symbol_table *s) {
  s->symbols = malloc(INIT_S_TABLE_SIZE * sizeof(symbol));
  s->max_entries = INIT_S_TABLE_SIZE;
  s->entry_count = 0;
}

symbol *getSymbol(const symbol_table *s, const char *name) {
  assert(s != NULL);

  /* simple linear search by name */
  for (int i = 0; i < s->entry_count; i++) {
    if (strcmp(s->symbols[i].name, name) == 0) {
      return s->symbols + i;
    }
  }

  return NULL;
}

void addSymbol(symbol_table *s, symbol entry) {
  if (s->entry_count == s->max_entries) {
    s->max_entries *= 2;
    if (!realloc(s->symbols, s->max_entries))
      printf("error!");
  }
  s->symbols[s->entry_count] = entry;
  s->entry_count += 1;
}

void freeTable(symbol_table *s) {
  free(s->symbols);
  free(s);
}
