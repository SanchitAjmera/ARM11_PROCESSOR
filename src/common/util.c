#include "util.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Takes in a string and returns a pointer to that string on heap */
char *strptr(const char *in) {
  char *out = malloc(sizeof(char) * (strlen(in) + 1));
  validatePtr(out, MEM_ASSIGN);
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

void extendString(resizableString_t *string) {
  if (string->length < string->maxLength - 1) {
    return;
  }
  while (string->length >= string->maxLength - 1) {
    string->maxLength *= RESIZE_FACTOR;
  }
  string->value =
      realloc(string->value, sizeof(*string->value) * string->maxLength);
  validatePtr(string->value, MEM_ASSIGN);
}

resizableString_t *newString(void) {
  resizableString_t *string = malloc(sizeof(resizableString_t));
  string->value = malloc(sizeof(char));
  validatePtr(string->value, MEM_ASSIGN);
  string->length = 0;
  string->maxLength = INITIAL_STRING_SIZE;
  extendString(string);
  return string;
}

void appendToString(resizableString_t *string, const char *append) {
  int oldLength = string->length;
  int appendLength = strlen(append);
  string->length += appendLength;
  extendString(string);
  char *startPos = string->value + oldLength;
  strcpy(startPos, append);
  strcpy(string->value + string->length, "\0");
}

void freeString(resizableString_t *string) {
  free(string->value);
  free(string);
}
