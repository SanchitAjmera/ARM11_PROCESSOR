#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef enum { ERR_PASS = 1, ERR_FAIL = 0 } Error;

typedef uint32_t word;
typedef uint8_t byte;
typedef unsigned int uint;

extern char *strptr(char *in);
extern int validatePtr(const void *ptr, const char *error_msg);
extern void errorExit(Error error);

#endif
