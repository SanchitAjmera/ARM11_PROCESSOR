#include "file_lines.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addLine(file_lines *fl, char *line) {
  printf("new line: %s\n", line);
  if (fl->lineCount == fl->maxLines) {
    fl->lines = realloc(fl->lines, fl->maxLines *= 2);
  }
  // Have to allocate memory on heap for each string
  fl->lines[fl->lineCount] = malloc(sizeof(char) * (strlen(line) + 1));
  strncpy(fl->lines[fl->lineCount], line, strlen(line));
  fl->lineCount++;
}

void addLines(file_lines *fl, char **lines, uint n) {
  for (int i = 0; i < n; i++) {
    addLine(fl, lines[i]);
  }
}

void printFileLines(file_lines *fileLines) {
  for (int i = 0; i < fileLines->lineCount; i++) {
    printf("%i: %s\n", i, fileLines->lines[i]);
  }
}

file_lines *newFileLines() {
  file_lines *fileLines = malloc(sizeof(file_lines));
  assert(fileLines != NULL);
  fileLines->maxLines = INIT_FILE_LINES_SIZE;
  fileLines->lineCount = 0;
  fileLines->lines = malloc(sizeof(char *) * fileLines->maxLines);
  assert(fileLines->lines != NULL);
  return fileLines;
}

void freeFileLines(file_lines *fileLines) {
  for (int i = 0; i < fileLines->lineCount; i++) {
    free(fileLines->lines[i]);
  }
  free(fileLines->lines);
  free(fileLines);
}
