#ifndef BOSS_H
#define BOSS_H

#include <stdbool.h>

typedef struct {
  char **questions;
  char **answers;
} passive_t;

typedef struct {
  int attack;
  int special;
  char *attackName;
  char *specialName;
  int health;
  int stamina;
} aggressive_t;

typedef struct {
  char *name;
  bool isPassive;
  union {
    passive_t *teaching;
    aggressive_t *fighting;
  }
} boss_t;

#endif
