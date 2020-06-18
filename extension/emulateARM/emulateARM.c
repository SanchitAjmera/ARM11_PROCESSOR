#include "emulateARM.h"
#include "../../src/assembler/assemble_constants.h"
#include "../../src/assembler/assemble_util.h"
#include "../../src/assembler/symbol_table.h"
#include "../../src/common/constants.h"
#include "../../src/common/util.h"
#include "../../src/emulator/decode/emulate_decode.h"
#include "../../src/emulator/emulate_util.h"
#include "../../src/emulator/execute/emulate_execute.h"
#include "../../src/emulator/fetch/emulate_fetch.h"
#include <stdio.h>
#include <string.h>

// Initialises an empty ARM state
static void initialiseArm(arm_t *arm) {
  arm->memory = calloc(50, sizeof(byte));
  arm->registers = calloc(NUM_REGISTERS, sizeof(word));
  arm->fetched = 0;
  arm->decoded.isSet = false;
}

// Initialises the symbol table with predefined symbols
// TODO: expand this function to take in a set of allowed instructions?
static void initialiseSymbolTable(symbol_table *s) {
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

static void firstPass(const char *code, symbol_table *symbolTable,
                      file_lines *output) {
  file_lines *expressions = newFileLines();

  /* Scan file for labels and expressions */
  char *codeCopy = strptr(code);
  char *rest = NULL;

  char *line = strtok_r(codeCopy, "\n", &rest);
  while (line != NULL) {
    // Iterate through chars in line
    bool isLabel = false;
    char *lineCopy = strptr(line);
    for (int i = 0; i < strlen(line); i++) {
      if (lineCopy[i] == ':') { // Line is a label
        char *label = strtok(lineCopy, ":");
        symbol labelSymbol = {strptr(label), LABEL,
                              .body.address =
                                  output->lineCount * WORD_SIZE_BYTES};
        addSymbol(symbolTable, labelSymbol);
        isLabel = true;
        break;                         // Line contains only one label
      } else if (lineCopy[i] == '=') { // Line contains an =0x expression
        char *expression =
            strtok(lineCopy + i - 1, " ],\n"); // Pulls expression out
        // =0x........ is only added to end of file if greater than 0xFF
        if (parseImmediate(expression + 1) > MAX_BYTE) {
          addLine(expressions, expression);
        }
        break; // Each line contains up to one expression. Remove if incorrect
      }
    }
    if (!isLabel) {
      char *lineStripped;
      if ((lineStripped = strtok(line, "\n"))) {
        addLine(output, lineStripped); // Adds line stripped of \n
      }
    }
    free(lineCopy);
    line = strtok_r(NULL, "\n", &rest);
  }

  /* Now that we have added all lines to output, we can add all expressions
     to the symbol table and calculate their address using lineCount */
  for (int i = 0; i < expressions->lineCount; i++) {
    char *expr = expressions->lines[i];
    word address = (output->lineCount + i) * WORD_SIZE_BYTES;
    symbol exprSymbol = {strptr(expr), LABEL, .body.address = address};
    addSymbol(symbolTable, exprSymbol);
  }

  // Add expressions to the end of the file
  addLines(output, expressions->lines, expressions->lineCount);

  freeFileLines(expressions);
  free(codeCopy);
}

/* Returns the register values as a result of running the user's code,
    line by line. Takes in the entirety of the code, with each line
    separated by a newline \n */
char *runCode(const char *code) {
  arm_t *armState = malloc(sizeof(arm_t));
  initialiseArm(armState);

  symbol_table *symbolTable = newSymbolTable();
  initialiseSymbolTable(symbolTable);

  file_lines *codeLines = newFileLines();
  // Perform first pass through code
  firstPass(code, symbolTable, codeLines);

  // Load the parsed code into ARM memory
  for (int i = 0; i < codeLines->lineCount; i++) {
    word lineParsed =
        parseLine(symbolTable, codeLines->lines[i], i * WORD_SIZE_BYTES);
    // Load the word lineParsed into memory byte by byte
    for (int j = 0; j < WORD_SIZE_BYTES; j++) {
      armState->memory[i * WORD_SIZE_BYTES + j] = lineParsed >> BYTE * j;
    }
  }

  decoded_t *decoded = malloc(sizeof(decoded_t));
  // PIPELINE
  while ((armState->decoded.isSet && armState->decoded.instruction) ||
         !armState->decoded.isSet) {
    execute(armState, decoded);
    decode(armState, decoded);
    fetch(armState);
  }

  // Construct the output strings
  resizable_string *output = newString();
  for (int i = 0; i < NUM_REGISTERS; i++) {
    char printLine[32];
    if (i == PC) {
      sprintf(printLine, "PC  : %10s (0x%s)\n", "N/A", "????????");
    } else if (i == CPSR) {
      sprintf(printLine, "CPSR: %10i (0x%08x)\n", armState->registers[i],
              armState->registers[i]);
    } else {
      sprintf(printLine, "R%-3i: %10i (0x%08x)\n", i, armState->registers[i],
              armState->registers[i]);
    }
    appendToString(output, printLine);
  }

  char *outputValue = strptr(output->value);

  free(armState->memory);
  free(armState->registers);
  free(armState);
  free(decoded);
  freeString(output);
  freeTable(symbolTable);
  freeFileLines(codeLines);
  return outputValue;
}

void freeOutput(char **output) {
  for (int i = 0; i < NUM_REGISTERS; i++) {
    free(output[i]);
  }
  free(output);
}

void printOutput(char **output) {
  for (int i = 0; i < NUM_REGISTERS; i++) {
    printf("%s", output[i]);
  }
}
