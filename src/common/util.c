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

void extendString(resizable_string *string) {
  if (string->length < string->maxLength - 1) {
    return;
  }
  while (string->length >= string->maxLength - 1) {
    string->maxLength *= 2;
  }
  string->value = realloc(string->value, sizeof(char) * string->maxLength);
}

resizable_string *newString(void) {
  resizable_string *string = malloc(sizeof(resizable_string));
  string->value = malloc(sizeof(char));
  string->length = 0;
  string->maxLength = 16;
  extendString(string);
  return string;
}

void appendToString(resizable_string *string, const char *append) {
  int oldLength = string->length;
  int appendLength = strlen(append);
  string->length += appendLength;
  extendString(string);
  char *startPos = string->value + oldLength;
  strcpy(startPos, append);
  strcpy(string->value + string->length, "\0");
}

void freeString(resizable_string *string) {
  free(string->value);
  free(string);
}
