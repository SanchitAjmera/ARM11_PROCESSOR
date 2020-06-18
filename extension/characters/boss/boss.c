#include "boss.h"
#include "../../../src/common/util.h"
#include "../../battle/battle.h"
#include "../../emulate_ARM/emulate_ARM.h"
#include "../../game_utils/game_util.h"
#include "../player/player.h"
#include "boss_constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static lookupBoss_t lookupBoss(const char *name) {
  for (int i = 0; i < BOSSES; i++) {
    if (strcmp(bossTable[i].key, name) == 0) {
      return bossTable[i];
    }
  }
  return {NULL, NULL, NULL};
}

static aggressive_t *createAggressive(const char *name) {
  aggressive_t *aggressive = malloc(sizeof(*aggressive));
  checkPtr(aggressive);
  checkPtr(questions);
  checkPtr(answers);
  if (strcmp(name, "Tony")) {
    aggressive_t tonyBattle = {TONY_ATTACK,      TONY_SPECIAL,
                               TONY_ATTACK_NAME, TONY_SPECIAL_NAME,
                               TONY_MAX_HEALTH,  TONY_MAX_HEALTH};
    boss->fighting = &tonyBattle;
  } else {
    aggressive_t kgkBattle = {KGK_ATTACK,       KGK_SPECIAL,    KGK_ATTACK_NAME,
                              KGK_SPECIAL_NAME, KGK_MAX_HEALTH, KGK_MAX_HEALTH};
    boss->fighting = &kgkBattle;
  }
  return aggressive;
}

static passive_t *createPassive(const char **questions, const char **answers) {
  passive_t *passive = malloc(sizeof(*passive));
  checkPtr(passive);
  checkPtr(table.questions);
  checkPtr(answers);
  passive->questions = table.questions;
  passive->answers = table.answers;
  passive->num = MAX_QUESTIONS;
  return passive;
}

// function to create and initialise a pointer to boss_t on the heap
boss_t *createBoss(const char *name) {
  // PRE: name is one of the pre-defined bosses
  boss_t *boss = malloc(sizeof(*boss));
  checkPtr(boss);
  boss->name = table.key;
  boss->teaching = createPassive(table.questions, table.answers);
  boss->fighting = createAggressive(name);
  return boss;
}

static void freeBossFighting(aggressive_t *aggressive) {
  if (aggressive == NULL) {
    return;
  }
  free((char *)aggressive->attackName);
  free((char *)aggressive->specialName);
  free(aggressive);
}

static void freeBossTeaching(passive_t *passive) {
  if (passive == NULL) {
    return;
  }
  free(passive);
}

void freeBoss(boss_t *boss) {
  freeBossTeaching(boss->teaching);
  freeBossFighting(boss->fighting);
  free((char *)boss->name);
  free(boss);
}

static void initBattle(boss_t *boss, player_t *player) {
  boss->fighting = malloc(sizeof(*boss->fighting));
  NULL_POINTER(boss->fighting);
  boss->fighting = malloc(sizeof(*boss->fighting));
  NULL_POINTER(boss->fighting);
  if (strcmp(boss->name, "Tony")) {
    aggressive_t tonyBattle = {TONY_ATTACK,      TONY_SPECIAL,
                               TONY_ATTACK_NAME, TONY_SPECIAL_NAME,
                               TONY_MAX_HEALTH,  TONY_MAX_HEALTH};
    (boss->fighting) = &tonyBattle;
  } else {
    aggressive_t kgkBattle = {KGK_ATTACK,       KGK_SPECIAL,    KGK_ATTACK_NAME,
                              KGK_SPECIAL_NAME, KGK_MAX_HEALTH, KGK_MAX_HEALTH};
    (boss->fighting) = &kgkBattle;
  }
}

// takes in the user's input for the answer
static char *getAnswer(void) {
  bool takeInput = true;
  resizable_string *code = newString();
  char *input;
  int size = sizeof(*input) * MAX_LINE_CHARACTERS;
  *input = malloc(size);
  checkPtr(input);
  while (takeInput) {
    printf("                     >> ");
    fgets(input, size - 1, stdin);
    // Check if "END" was typed in
    char *copy = strptr(input);
    char *token = strtok(copy, " \n");
    free(copy);
    if (strcmp(token, "END") == 0) {
      takeInput = false;
    } else {
      appendToString(code, input);
    }
  }
  char *output = runCode(code->value);
  printf("%s\n", output);
  freeString(code);
  free(input);
  return output;
}

static void processResult(boss_t *boss, player_t *player, bool correct) {
  // player can attack boss if correct
  if (correct) {
    printf("                           Well done you answered correctly\n");
    printf("                           You can attack %s\n", boss->name);
  }
  // boss attacks player if incorrect
  else {
    printf("%s gets aggravated due to your incorrect answer...\n", boss->name);
  }
  battle(boss, player);
}

// function to start the quiz on assembly code
static void quiz(boss_t *boss, player_t *player) {
  // PRE: boss->teaching has been initialised
  bool correct = false;
  printf("Wild %s appeared!\n%s starts asking you assembly questions!\n",
         boss->name, boss->name);
  srand(time(NULL));
  int randomQuestion = rand() % MAX_QUESTIONS;
  printf("                        %s asks: %s\n", boss->name,
         boss->teaching->questions[randomQuestion]);
  printf("                        enter 'END' on a separate line to submit "
         "your answer\n");
  const char *input = getAnswer();
  const char *correctInput = runCode(boss->teaching->answers[randomQuestion]);
  printf("%s", correctInput);
  if (strcmp(input, correctInput) == 0) {
    correct = true;
  }
  processResult(boss, player, correct);
}

void fight(state *currentState, char *boss) {
  if (IS_NULL(currentState->currentRoom->boss)) {
    printf("          No Boss in da house, go to Central Lab...\n");
    return;
  }
  if (strcmp(boss, currentState->currentRoom->boss->name) != 0) {
    printf("there is no boss by that name ");
    return;
  }
  quiz(currentState->currentRoom->boss, currentState->player);
}
