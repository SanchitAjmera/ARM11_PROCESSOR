#ifndef EMULATE_ARM_H
#define EMULATE_ARM_H

extern void printOutput(char **output);
extern void freeOutput(char **output);
extern char *runCode(const char *code);

#endif
