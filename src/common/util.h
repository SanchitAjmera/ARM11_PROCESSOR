#ifndef UTIL_H
#define UTIL_H

typedef enum { ERR_PASS = 1, ERR_FAIL = 0 } Error;

extern char *strptr(char *in);
extern int validatePtr(const void *ptr, const char *error_msg);
extern void errorExit(Error error);

#endif
