#ifndef BOSS_H
#define BOSS_H

#include <stdbool.h>
#include <string.h>

#define MIN_QUESTIONS_CORRECT (3)
#define MAX_QUESTIONS (5)
#define BOSSES (2)
#define KGK_ATTACK ()
#define KGK_SPECIAL ()
#define KGK_ATTACK_NAME ()
#define KGK_SPECIAL_NAME ()
#define KGK_MAX_HEALTH ()
#define TONY_ATTACK ()
#define TONY_SPECIAL ()
#define TONY_ATTACK_NAME ()
#define TONY_SPECIAL_NAME ()
#define TONY_MAX_HEALTH ()

typedef struct {
  const char **questions;
  const char **answers;
  int num;
} passive_t;

typedef struct {
  int attack;
  int special;
  const char *attackName;
  const char *specialName;
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
  } state;
} boss_t;

typedef struct {
  const char *key;
  boss_t *(*create)(const char **, const char **);
  const char **questions;
  const char **answers;
  aggressive_t fightingState;
} lookupBoss_t;

// variables for KGK boss
static const kgkQuestions[MAX_QUESTIONS][] = {};
static const kgkAnswers[MAX_QUESTIONS][] = {};
static const aggressive_t kgkBattle = {KGK_ATTACK,      KGK_SPECIAL,
                                       KGK_ATTACK_NAME, KGK_SPECIAL_NAME,
                                       KGK_MAX_HEALTH,  KGK_MAX_HEALTH};
// variables for TONY boss
static const tonyQuestions[MAX_QUESTIONS][] = {};
static const tonyAnswers[MAX_QUESTIONS][] = {};
static const aggressive_t tonyBattle = {TONY_ATTACK,      TONY_SPECIAL,
                                        TONY_ATTACK_NAME, TONY_SPECIAL_NAME,
                                        TONY_MAX_HEALTH,  TONY_MAX_HEALTH};
// lookup table for bosses
static const lookupBoss_t bossTable[] = {
    {"Konstantinos", createKGK, kgkQuestions, kgkAnswers, kgkBattle}};

lookupBoss_t lookup(const char *name) {
  for (int i = 0; i < BOSSES; i++) {
    if (strcmp(bossTable[i].key, name) == 0) {
      return bossTable[i];
    }
  }
  return NULL;
}

#endif
