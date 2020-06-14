#ifndef BOSS_H
#define BOSS_H

#include <stdbool.h>

#define MIN_QUESTIONS_CORRECT (3)
#define KGK_SIZE (5)

typedef struct {
  const char **questions;
  const char **answers;
  int num;
} passive_t;

typedef struct {
  int attack;
  int special;
  char *attackName;
  char *specialName;
  int health;
  const int maxHealth;
} aggressive_t;

typedef struct {
  char *name;
  // default: true
  bool isPassive;
  union {
    passive_t *teaching;
    aggressive_t *fighting;
  }
} boss_t;

static const char *kgkQuestions[] = {};
static const char *kgkAnswers[] = {};

#endif
