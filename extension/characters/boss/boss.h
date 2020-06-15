#ifndef BOSS_H
#define BOSS_H

#include "../player/player.h"
#include "boss_constants.h"
#include <stdbool.h>
#include <string.h>

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
static const char *kgkQuestions[MAX_QUESTIONS] = {"", "", "", "", ""};
static const char *kgkAnswers[MAX_QUESTIONS] = {"", "", "", "", ""};
static const aggressive_t kgkBattle = {KGK_ATTACK,      KGK_SPECIAL,
                                       KGK_ATTACK_NAME, KGK_SPECIAL_NAME,
                                       KGK_MAX_HEALTH,  KGK_MAX_HEALTH};
// variables for TONY boss
static const char *tonyQuestions[MAX_QUESTIONS] = {"", "", "", "", ""};
static const char *tonyAnswers[MAX_QUESTIONS] = {"", "", "", "", ""};
static const aggressive_t tonyBattle = {TONY_ATTACK,      TONY_SPECIAL,
                                        TONY_ATTACK_NAME, TONY_SPECIAL_NAME,
                                        TONY_MAX_HEALTH,  TONY_MAX_HEALTH};
// lookup table for bosses
static const lookupBoss_t bossTable[] = {
    {"Konstantinos", kgkQuestions, kgkAnswers, kgkBattle},
    {"Tony", tonyQuestions, tonyAnswers, tonyBattle}};

extern boss_t *createBoss(const char *name);
// for testing
extern passive_t *createPassive(const char **questions, const char **answers,
                                int num);
extern boss_t *initBoss(const char *name);
extern void freeBossFighting(aggressive_t *aggressive);
extern void freeBossTeaching(passive_t *passive);
extern void freeBoss(boss_t *boss);
extern void initBattle(boss_t *boss, player_t *player);
extern void processResult(boss_t *boss, player_t *player);
extern void quiz(boss_t *boss, player_t *player);

#endif
