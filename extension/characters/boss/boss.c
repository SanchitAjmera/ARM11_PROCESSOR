#include "boss.h"
#include "../../../src/common/util.h"
#include "../../battle/battle.h"
#include "../../emulate_ARM/emulate_ARM.h"
#include "../player/player.h"
#include "boss_constants.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// dummy pre-processor function
#define NULL_POINTER(pointer) (checkPtr(pointer))
// dummy function
lookupBoss_t lookupBoss(const char *name) {
  for (int i = 0; i < BOSSES; i++) {
    if (strcmp(bossTable[i].key, name) == 0) {
      return bossTable[i];
    }
  }
  assert(false);
}

passive_t *createPassive(const char **questions, const char **answers,
                         int num) {
  passive_t *passive = malloc(sizeof(*passive));
  NULL_POINTER(passive);
  NULL_POINTER(questions);
  NULL_POINTER(answers);
  passive->questions = questions;
  passive->answers = answers;
  passive->num = MAX_QUESTIONS;
  return passive;
}

// function to create and initialise a pointer to boss_t on the heap
boss_t *initBoss(const char *name) {
  boss_t *boss = malloc(sizeof(*boss));
  NULL_POINTER(boss);
  NULL_POINTER(name);
  boss->name = name;
  // defaults to true
  boss->isPassive = true;
  boss->state = malloc(sizeof(*boss->state));
  NULL_POINTER(boss->state);
  boss->state->teaching = NULL;
  return boss;
}

boss_t *createBoss(const char *name) {
  // PRE: name is one of the pre-defined bosses
  lookupBoss_t table = lookupBoss(name);
  boss_t *boss = initBoss(table.key);
  boss->state->teaching =
      createPassive(table.questions, table.answers, MAX_QUESTIONS);
  return boss;
}

void freeBossFighting(aggressive_t *aggressive) {
  if (aggressive == NULL) {
    return;
  }
  free((void *)aggressive->attackName);
  free((void *)aggressive->specialName);
  free(aggressive);
}

void freeBossTeaching(passive_t *passive) {
  if (passive == NULL) {
    return;
  }
  // for (int i = 0; i < passive->num; i++) {
  // free((char *)passive->questions[i]);
  // free((char *)passive->answers[i]);
  // }
  // free((char **)passive->questions);
  // free((char **)passive->answers);
  // free(passive);
}

void freeBoss(boss_t *boss) {
  freeBossTeaching(boss->state->teaching);
  freeBossFighting(boss->state->fighting);
  free((void *)boss->name);
  free(boss);
}

void initBattle(boss_t *boss, player_t *player, bool correct) {
  freeBossTeaching(boss->state->teaching);
  boss->isPassive = false;
  boss->state->fighting = malloc(sizeof(*boss->state->fighting));
  NULL_POINTER(boss->state->fighting);
  boss->state->fighting = malloc(sizeof(*boss->state->fighting));
  NULL_POINTER(boss->state->fighting);
  if (strcmp(boss->name, "Tony")) {
    aggressive_t tonyBattle = {TONY_ATTACK,      TONY_SPECIAL,
                               TONY_ATTACK_NAME, TONY_SPECIAL_NAME,
                               TONY_MAX_HEALTH,  TONY_MAX_HEALTH};
    (boss->state->fighting) = &tonyBattle;
  } else {
    aggressive_t kgkBattle = {KGK_ATTACK,       KGK_SPECIAL,    KGK_ATTACK_NAME,
                              KGK_SPECIAL_NAME, KGK_MAX_HEALTH, KGK_MAX_HEALTH};
    (boss->state->fighting) = &kgkBattle;
  }
  battle(boss, player, correct);
}

// takes in the user's input for the answer
static char *getAnswer(void) {
  // TODO: get answer from user as input
  bool takeInput = true;
  resizableString_t *code = newString();

  char *input = malloc(sizeof(char) * 50);
  while (takeInput) {
    printf("                  >> ");
    fgets(input, sizeof(char) * 50, stdin);

    // Check if "END" was typed in
    char *inputCopy = strptr(input);
    char *token = strtok(inputCopy, " \n");
    if (!strcmp(token, "END")) {
      takeInput = false;
      free(inputCopy);
      break;
    }
    free(inputCopy);

    appendToString(code, input);
  }
  // printf("%s\n", code->value);
  char *output = runCode(code->value);
  printf("%s", output);

  freeString(code);
  free(input);
  printf(">");
  return output;
}

void processResult(boss_t *boss, player_t *player, bool correct) {
  // TODO: special KGK case
  /*
  printf("You scored %d correct out of %d.\n", correct, MAX_QUESTIONS);
  if (correct > MIN_QUESTIONS_CORRECT) {
    printf("%s is happy you scored well!\n", boss->name);
    return;
  }
  */

  // the case where the answer is correct
  if (correct) {
    printf("                           Well done you answered correctly\n");
    printf("                           You can attack %s\n", boss->name);
  }
  // case when the answer is incorrect
  else {
    printf("%s gets aggravated due to your low score...\n", boss->name);
    printf("%s says: mitigations won't save you this time!\n", boss->name);
  }
  // if small number of questions correct start turn-based comabat
  initBattle(boss, player, correct);
}

// function to start the quiz on assembly code
void quiz(boss_t *boss, player_t *player) {
  // PRE: boss->teaching has been initialised
  bool correct = false;
  system(CLEAR);
  printf("\n\n\n\n\n\n\n");
  printf("                  Wild %s appeared!\n\n                  %s "
         "starts asking "
         "you assembly questions!\n\n",
         boss->name, boss->name);
  srand(time(NULL));
  int randomQuestion = rand() % 5;

  printf("                  Question: %s\n\n",
         boss->state->teaching->questions[randomQuestion]);
  printf("                  enter 'END' on a separate line to submit "
         "your answer\n\n");

  const char *input = getAnswer();
  system(CLEAR);
  char *expectedOutput =
      runCode(boss->state->teaching->answers[randomQuestion]);
  printf("\n\n\n");
  printf("Your Output:\n\n");
  printf("%s\n\n", input);
  printf("Correct Output:\n\n");
  printf("%50s", expectedOutput);
  if (strcmp(input, expectedOutput) == 0) {
    correct = true;
  }

  processResult(boss, player, correct);
}

void fight(state *currentState, char *boss) {

  if (!currentState->currentRoom->boss) {
    printf("          No Boss in da house, go to Central Lab...\n");
    return;
  }
  if (strcmp(boss, currentState->currentRoom->boss->name)) {
    printf("there is no boss by that name ");
    return;
  }
  quiz(currentState->currentRoom->boss, currentState->player);
}
