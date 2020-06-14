#ifndef BOSS_H
#define BOSS_H

#include <stdbool.h>
#include <string.h>

#define MIN_QUESTIONS_CORRECT (3)
#define MAX_QUESTIONS (5)
#define BOSSES (2)
#define KGK_ATTACK (15)
#define KGK_SPECIAL (25)
#define KGK_ATTACK_NAME ("10 mins remaining")
#define KGK_SPECIAL_NAME ("LabTS is down")
#define KGK_MAX_HEALTH (120)
#define TONY_ATTACK (20)
#define TONY_SPECIAL (1000)
#define TONY_ATTACK_NAME ("Code style violations")
#define TONY_SPECIAL_NAME ("Minus 1000 marks")
#define TONY_MAX_HEALTH (90)

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
    {"Konstantinos", kgkQuestions, kgkAnswers, kgkBattle},
    {"Tony", tonyQuestions, tonyAnswers, tonyBattle}};

lookupBoss_t lookup(const char *name) {
  for (int i = 0; i < BOSSES; i++) {
    if (strcmp(bossTable[i].key, name) == 0) {
      return bossTable[i];
    }
  }
  return NULL;
}

#endif
