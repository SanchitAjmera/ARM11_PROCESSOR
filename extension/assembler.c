#include "../src/assembler/assemble_util.h"
#include "../src/assembler/symbol_table.h"
#include "../src/emulator/emulate_util.h"
#include "../src/emulator/execute/emulate_execute.h"
#include <stdio.h>
#include <string.h>

// Initialises an empty ARM state
void initialiseArm(arm_t *arm) {
  arm->memory = calloc(10, sizeof(byte));
  arm->registers = calloc(17, sizeof(word));
  arm->fetched = 0;
  arm->decoded.isSet = false;
}

// Initialises the symbol table with predefined symbols
void initialiseSymbolTable(symbol_table *s) {
  symbol predefinedSymbols[23] = {
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
  addSymbols(s, predefinedSymbols, 23);
}

void compileInstruction(char *instruction) {}

int main() {
  arm_t *armState = malloc(sizeof(arm_t));
  initialiseArm(armState);

  symbol_table *symbolTable = newSymbolTable();
  initialiseSymbolTable(symbolTable);

  for (int i = 0; i < 17; i++) {
    printf("R%i: 0x%08x\n", i, armState->registers[i]);
  }

  free(armState->memory);
  free(armState->registers);
  free(armState);
  freeTable(symbolTable);

  printf("ehehe\n");
  return EXIT_SUCCESS;
}
