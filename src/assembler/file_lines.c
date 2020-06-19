#include "file_lines.h"
<<<<<<< HEAD
#include <assert.h>
=======
#include "../common/util.h"
#include "assemble_constants.h"
>>>>>>> code-cleanup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
void addLine(file_lines *fl, char *line) {
  printf("new line: %s\n", line);
=======
void addLine(fileLines_t *fl, const char *line) {
>>>>>>> code-cleanup
  if (fl->lineCount == fl->maxLines) {
    fl->lines = realloc(fl->lines, fl->maxLines *= 2);
    validatePtr(fl->lines, MEM_ASSIGN);
  }
  // Have to allocate memory on heap for each string
  fl->lines[fl->lineCount] = malloc(sizeof(char) * (strlen(line) + 1));
  strncpy(fl->lines[fl->lineCount], line, strlen(line));
  fl->lineCount++;
}

<<<<<<< HEAD
void addLines(file_lines *fl, char **lines, uint n) {
=======
/* Adds each line into file line structure from string array */
void addLines(fileLines_t *fl, char **lines, uint n) {
>>>>>>> code-cleanup
  for (int i = 0; i < n; i++) {
    addLine(fl, lines[i]);
  }
}

<<<<<<< HEAD
void printFileLines(file_lines *fileLines) {
=======
/* Outputs each line stored in the file lines structure */
void printFileLines(fileLines_t *fileLines) {
>>>>>>> code-cleanup
  for (int i = 0; i < fileLines->lineCount; i++) {
    printf("%i: %s\n", i, fileLines->lines[i]);
  }
}

<<<<<<< HEAD
file_lines *newFileLines() {
  file_lines *fileLines = malloc(sizeof(file_lines));
  assert(fileLines != NULL);
=======
/* Initialising the structure to store each line of instruction file */
fileLines_t *newFileLines(void) {
  fileLines_t *fileLines = malloc(sizeof(*fileLines));
  validatePtr(fileLines, MEM_ASSIGN);
>>>>>>> code-cleanup
  fileLines->maxLines = INIT_FILE_LINES_SIZE;
  fileLines->lineCount = 0;
  fileLines->lines = malloc(sizeof(*fileLines->lines) * fileLines->maxLines);
  validatePtr(fileLines->lines, MEM_ASSIGN);
  return fileLines;
}

<<<<<<< HEAD
void freeFileLines(file_lines *fileLines) {
=======
/* Free resources used for storing program file lines */
void freeFileLines(fileLines_t *fileLines) {
>>>>>>> code-cleanup
  for (int i = 0; i < fileLines->lineCount; i++) {
    free(fileLines->lines[i]);
  }
  free(fileLines->lines);
  free(fileLines);
}
