#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Takes in a string and returns a pointer to that string on heap */
<<<<<<< HEAD
char *strptr(char *in) {
  char *out = malloc(sizeof(char) * (strlen(in)) + 1);
=======
char *strptr(const char *in) {
  char *out = malloc(sizeof(char) * (strlen(in) + 1));
  validatePtr(out, MEM_ASSIGN);
>>>>>>> code-cleanup
  strcpy(out, in);
  return out;
}

<<<<<<< HEAD
Error validatePtr(const void *ptr, const char *error_msg) {
  if (ptr == NULL) {
    printf("Error: %s\n", error_msg);
    return ERR_FAIL;
=======
/* Checks for failed memory allocation */
void validatePtr(const void *ptr, Error error) {
  if (ptr == NULL) {
    printf("Error: NULL pointer.");
    errorExit(error);
>>>>>>> code-cleanup
  }
}

<<<<<<< HEAD
void errorExit(Error error) {
  if (error == ERR_FAIL) {
    exit(EXIT_FAILURE);
  }
}
=======
/* Exits program if an error occurred */
void errorExit(Error error) { exit(error); }
>>>>>>> code-cleanup
