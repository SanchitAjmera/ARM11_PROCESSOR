#include "assembler/assemble_util.h"
#include "assembler/file_lines.h"
#include "assembler/symbol_table.h"
#include "common/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREDEFINED_SYMBOLS_COUNT 23

int main(int argc, char **argv) {
  symbol_table *symbolTable = newSymbolTable();
  symbol predefinedSymbols[PREDEFINED_SYMBOLS_COUNT] = {
      {strptr("add"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("sub"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("rsb"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("and"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("eor"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("orr"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("mov"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("tst"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("teq"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("cmp"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("mul"), INSTR, .body.assembleFunc = assembleMultiply},
      {strptr("mla"), INSTR, .body.assembleFunc = assembleMultiply},
      {strptr("ldr"), INSTR, .body.assembleFunc = assembleSDTI},
      {strptr("str"), INSTR, .body.assembleFunc = assembleSDTI},
      {strptr("beq"), INSTR, .body.assembleFunc = assembleBranch},
      {strptr("bne"), INSTR, .body.assembleFunc = assembleBranch},
      {strptr("bge"), INSTR, .body.assembleFunc = assembleBranch},
      {strptr("blt"), INSTR, .body.assembleFunc = assembleBranch},
      {strptr("bgt"), INSTR, .body.assembleFunc = assembleBranch},
      {strptr("ble"), INSTR, .body.assembleFunc = assembleBranch},
      {strptr("b"), INSTR, .body.assembleFunc = assembleBranch},
      {strptr("lsl"), INSTR, .body.assembleFunc = assembleDPI},
      {strptr("andeq"), INSTR, .body.assembleFunc = assembleDPI}};
  addSymbols(symbolTable, predefinedSymbols, PREDEFINED_SYMBOLS_COUNT);
  file_lines *fileLines = newFileLines();

  FILE *armFile = fopen(argv[1], "r");
  scanFile(armFile, symbolTable, fileLines);
  fclose(armFile);

  printFileLines(fileLines);
  printSymbolTable(symbolTable);

  printf("\n");

  char buffer[512];
  strcpy(buffer, "mov r1,#1\nadd r2,r1,#2\nfoo:\nldr r0,[r1,r2,lsl #2]\n");
  char *delim = "[]";

  char *word = strtok(buffer, delim);
  while (word != NULL) {
    printf("%s\n", word);
    word = strtok(NULL, delim);
  }

  FILE *binOutFile = fopen("out.bin", "wb");
  parseLines(fileLines, symbolTable, binOutFile);
  fclose(binOutFile);

  freeTable(symbolTable);
  freeFileLines(fileLines);

  return EXIT_SUCCESS;
}
