#ifndef BOSS_H
#define BOSS_H

#include <stdbool.h>

#define MIN_QUESTIONS_CORRECT (3)
#define KGK_SIZE ()
#define KGK_ATTACK ()
#define KGK_SPECIAL ()
#define KGK_ATTACK_NAME ()
#define KGK_SPECIAL_NAME ()
#define KGK_MAX_HEALTH ()

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

static const kgkQuestions[KGK_SIZE][] = {};
static const kgkAnswers[KGK_SIZE][] = {};
static const aggressive_t kgkBattle = {KGK_ATTACK,      KGK_SPECIAL,
                                       KGK_ATTACK_NAME, KGK_SPECIAL_NAME,
                                       KGK_MAX_HEALTH,  KGK_MAX_HEALTH};

#endif
