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
Error validatePtr(const void *ptr, const char *errorMsg) {
  if (ptr == NULL) {
    printf("Error: %s\n", errorMsg);
    return ERR_FAIL;
  }
  return ERR_PASS;
}

/* Exits program if an error occurred */
void errorExit(Error error) {
  if (error == ERR_FAIL) {
    exit(EXIT_FAILURE);
  }
}
