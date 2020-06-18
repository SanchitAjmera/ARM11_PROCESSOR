#ifndef BOSS_H
#define BOSS_H
typedef struct boss_t boss_t;

#include "../../game_util.h"
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

typedef union {
  passive_t *teaching;
  aggressive_t *fighting;
} state_t;

struct boss_t {
  const char *name;
  // default: true
  bool isPassive;
  state_t *state;
};

typedef struct {
  const char *key;
  const char **questions;
  const char **answers;
  aggressive_t fightingState;
} lookupBoss_t;

// variables for KGK boss
static const char *kgkQuestions[MAX_QUESTIONS] = {
    "move 69 into register 1", "move 69 into register 1",
    "move 69 into register 1", "move 69 into register 1",
    "move 69 into register 1"};
static const char *kgkAnswers[MAX_QUESTIONS] = {
    "mov r1, #69", "mov r1, #69", "mov r1, #69", "mov r1, #69", "mov r1, #69"};
// static aggressive_t kgkBattle = {KGK_ATTACK,      KGK_SPECIAL,
//                                        KGK_ATTACK_NAME, KGK_SPECIAL_NAME,
//                                        KGK_MAX_HEALTH,  KGK_MAX_HEALTH};
// variables for TONY boss
static const char *tonyQuestions[MAX_QUESTIONS] = {"", "", "", "", ""};
static const char *tonyAnswers[MAX_QUESTIONS] = {"", "", "", "", ""};
// static aggressive_t tonyBattle = {TONY_ATTACK,      TONY_SPECIAL,
//                                         TONY_ATTACK_NAME, TONY_SPECIAL_NAME,
//                                         TONY_MAX_HEALTH,  TONY_MAX_HEALTH};
// lookup table for bosses
static const lookupBoss_t bossTable[] = {
    {"konstantinos", kgkQuestions, kgkAnswers},
    {"tony", tonyQuestions, tonyAnswers}};

extern boss_t *createBoss(const char *name);
// for testing
extern passive_t *createPassive(const char **questions, const char **answers,
                                int num);
extern boss_t *initBoss(const char *name);
extern void freeBossFighting(aggressive_t *aggressive);
extern void freeBossTeaching(passive_t *passive);
extern void freeBoss(boss_t *boss);
extern void initBattle(boss_t *boss, player_t *player, bool correct);
extern void processResult(boss_t *boss, player_t *player, bool correct);
extern void quiz(boss_t *boss, player_t *player);
extern void fight(state *currentState, char *boss);

#endif
