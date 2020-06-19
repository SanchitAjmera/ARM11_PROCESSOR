#ifndef FILE_LINES_H
#define FILE_LINES_H

#include "../common/constants.h"
#include "../common/util.h"

#define INIT_FILE_LINES_SIZE 16

/* Structure that stores the ARM file's lines as an array of strings */
typedef struct {
  char **lines;
  uint lineCount, maxLines;
} fileLines_t;

<<<<<<< HEAD
extern void addLine(file_lines *fl, char *line);
extern void addLines(file_lines *fl, char **lines, uint n);
extern void printFileLines(file_lines *fileLines);
extern file_lines *newFileLines();
extern void freeFileLines(file_lines *fileLines);
=======
extern void addLine(fileLines_t *fl, const char *line);
extern void addLines(fileLines_t *fl, char **lines, uint n);
extern void printFileLines(fileLines_t *fileLines);
extern fileLines_t *newFileLines(void);
extern void freeFileLines(fileLines_t *fileLines);
>>>>>>> code-cleanup

#endif
