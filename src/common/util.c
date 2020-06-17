#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Takes in a string and returns a pointer to that string on heap */
char *strptr(const char *in) {
  char *out = malloc(sizeof(char) * (strlen(in) + 1));
  strcpy(out, in);
  return out;
}

/* Checks for failed memory allocation */
void validatePtr(const void *ptr, Error error) {
  if (ptr == NULL) {
    printf("Error: NULL pointer.");
    errorExit(error);
  }
}

/* Exits program if an error occurred */
void errorExit(Error error) { exit(error); }
