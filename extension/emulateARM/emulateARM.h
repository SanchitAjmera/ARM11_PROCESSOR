#ifndef EMULATEARM_H
#define EMULATEARM_H

extern void printOutput(char **output);
extern void freeOutput(char **output);
extern char *runCode(const char *code);

#endif
