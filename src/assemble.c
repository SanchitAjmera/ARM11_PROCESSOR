<<<<<<< HEAD
#include "assembler/assemble_util.h"
=======
#include "assembler/assemble_constants.h"
>>>>>>> code-cleanup
#include "assembler/file_lines.h"
#include "assembler/symbol_table.h"
#include "assembler/utils/assemble_dpi.h"
#include "assembler/utils/assemble_util.h"
#include "common/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREDEFINED_SYMBOLS_COUNT 23

int main(int argc, char **argv) {
  symbol_table *symbolTable = newSymbolTable();
  symbol **pre = createSymbols(PREDEFINED_SYMBOLS_COUNT, sizeof(*pre));
  symbol predefinedSymbols[PREDEFINED_SYMBOLS_COUNT] = {
      {strptr("add"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("sub"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("rsb"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("and"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("eor"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("orr"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("mov"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("tst"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("teq"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("cmp"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("mul"), INSTR, 0, .body.assembleFunc = assembleMultiply},
      {strptr("mla"), INSTR, 0, .body.assembleFunc = assembleMultiply},
      {strptr("ldr"), INSTR, 0, .body.assembleFunc = assembleSDTI},
      {strptr("str"), INSTR, 0, .body.assembleFunc = assembleSDTI},
      {strptr("beq"), INSTR, 0, .body.assembleFunc = assembleBranch},
      {strptr("bne"), INSTR, 0, .body.assembleFunc = assembleBranch},
      {strptr("bge"), INSTR, 0, .body.assembleFunc = assembleBranch},
      {strptr("blt"), INSTR, 0, .body.assembleFunc = assembleBranch},
      {strptr("bgt"), INSTR, 0, .body.assembleFunc = assembleBranch},
      {strptr("ble"), INSTR, 0, .body.assembleFunc = assembleBranch},
      {strptr("b"), INSTR, 0, .body.assembleFunc = assembleBranch},
      {strptr("lsl"), INSTR, 0, .body.assembleFunc = assembleDPI},
      {strptr("andeq"), INSTR, 0, .body.assembleFunc = assembleDPI}};
  for (int i = 0; i < PREDEFINED_SYMBOLS_COUNT; i++) {
    pre[i][0] = predefinedSymbols[i];
  }
  addSymbols(symbolTable, pre, PREDEFINED_SYMBOLS_COUNT);
  fileLines_t *fileLines = newFileLines();

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

  FILE *binOutFile = fopen(argv[2], "wb");
  parseLines(fileLines, symbolTable, binOutFile);
  fclose(binOutFile);

  freeSymbols(pre, PREDEFINED_SYMBOLS_COUNT);
  freeTable(symbolTable);
  freeFileLines(fileLines);

  return EXIT_SUCCESS;
}
