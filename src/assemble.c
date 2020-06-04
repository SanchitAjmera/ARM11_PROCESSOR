#include "assemble_util.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  symbol_table *symbolTable = malloc(sizeof(symbol_table));
  initSymbolTable(symbolTable);

  FILE *armFile = fopen(argv[1], "r");
  file_lines *fileLines = scanFile(armFile, symbolTable);

  printFileLines(fileLines);

  fclose(armFile);

  printf("\n");

  char buffer[512];
  strcpy(buffer, "mov r1,#1\nadd r2,r1,#2\nfoo:\nldr r0,[r1,r2,lsl #2]\n");
  char *delim = "#";

  char *word = strtok(buffer, delim);
  while (word != NULL) {
    printf("%s\n", word);
    word = strtok(NULL, delim);
  }

  freeTable(symbolTable);
  freeFileLines(fileLines);

  return EXIT_SUCCESS;
}
