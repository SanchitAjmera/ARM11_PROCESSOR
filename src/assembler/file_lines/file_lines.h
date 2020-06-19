#ifndef FILE_LINES_H
#define FILE_LINES_H

#include "../../common/util.h"

// Structure that stores the ARM file's lines as an array of strings
typedef struct {
  char **lines;
  uint lineCount, maxLines;
} fileLines_t;

extern void addLine(fileLines_t *fl, const char *line);
extern void addLines(fileLines_t *fl, char **lines, uint n);
extern void printFileLines(fileLines_t *fileLines);
extern fileLines_t *newFileLines(void);
extern void freeFileLines(fileLines_t *fileLines);

#endif
