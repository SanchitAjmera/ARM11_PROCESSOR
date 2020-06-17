#include "file_lines.h"
#include "../common/util.h"
#include "assemble_constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addLine(file_lines *fl, const char *line) {
  if (fl->lineCount == fl->maxLines) {
    fl->lines = realloc(fl->lines, fl->maxLines *= 2);
    validatePtr(fl->lines, MEM_ASSIGN);
  }
  // Have to allocate memory on heap for each string
  fl->lines[fl->lineCount++] = strptr(line);
}

/* Adds each line into file line structure from string array */
void addLines(file_lines *fl, char **lines, uint n) {
  for (int i = 0; i < n; i++) {
    addLine(fl, lines[i]);
  }
}

/* Outputs each line stored in the file lines structure */
void printFileLines(file_lines *fileLines) {
  for (int i = 0; i < fileLines->lineCount; i++) {
    printf("%i: %s\n", i, fileLines->lines[i]);
  }
}

/* Initialising the structure to store each line of instruction file */
file_lines *newFileLines() {
  file_lines *fileLines = malloc(sizeof(*fileLines));
  validatePtr(fileLines, MEM_ASSIGN);
  fileLines->maxLines = INIT_FILE_LINES_SIZE;
  fileLines->lineCount = 0;
  fileLines->lines = malloc(sizeof(*fileLines->lines) * fileLines->maxLines);
  validatePtr(fileLines->lines, MEM_ASSIGN);
  return fileLines;
}

/* Free resources used for storing program file lines */
void freeFileLines(file_lines *fileLines) {
  for (int i = 0; i < fileLines->lineCount; i++) {
    free(fileLines->lines[i]);
  }
  free(fileLines->lines);
  free(fileLines);
}
