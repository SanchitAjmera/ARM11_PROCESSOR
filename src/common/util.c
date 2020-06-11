#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Takes in a string and returns a pointer to that string on heap */
char *strptr(char *in) {
  char *out = malloc(sizeof(char) * (strlen(in)) + 1);
  strcpy(out, in);
  return out;
}

Error validatePtr(const void *ptr, const char *errorMsg) {
  if (ptr == NULL) {
    printf("Error: %s\n", error_msg);
    return ERR_FAIL;
  }
  return ERR_PASS;
}

void errorExit(Error error) {
  if (error == ERR_FAIL) {
    exit(EXIT_FAILURE);
  }
}
