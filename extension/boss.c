#include "boss.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// dummy pre-processor function
#define NULL_POINTER(pointer) (validatePtr(pointer, "Not enough memory."))
// dummy function
void validatePtr(void *pointer, const char *) {}

passive_t *createPassive(const char **questions, const char **answers,
                         int num) {
  passive_t *passive = malloc(sizeof(*passive));
  NULL_POINTER(passive);
  NULL_POINTER(questions);
  NULL_POINTER(answers);
  passive->questions = questions;
  passive->answers = answers;
  return passive;
}

// function to create and initialise a pointer to boss_t on the heap
boss_t *createBoss(const char *name) {
  boss_t *boss = malloc(sizeof(*boss));
  NULL_POINTER(boss);
  NULL_POINTER(name);
  boss->name = name;
  // defaults to true
  boss->isPassive = true;
  boss->teaching = NULL;
}

void freeBossFighting(aggressive_t *aggressive) {
  if (aggressive == NULL) {
    return;
  }
  free(attackName);
  free(specialName);
  free(aggressive);
}

void freeBossTeaching(passive_t *passive) {
  if (passive == NULL) {
    return;
  }
  for (int i = 0; i < passive->num; i++) {
    free(passive->questions[i]);
    free(passive->answers[i]);
  }
  free(passive->questions);
  free(passive->answers);
  free(passive);
}

void freeBoss(boss_t *boss) {
  freeBossTeaching(boss->teaching);
  freeBossFighting(boss->fighting);
  free(boss->name);
  free(boss);
}

void initBattle(boss_t *boss, aggressive_t stats) {
  boss->isPassive = false;
  freeBossTeaching(boss->teaching);
  boss->fighting = malloc(sizeof(*boss->fighting));
  NULL_POINTER(boss->fighting);
  *boss->fighting = stats;
}

boss_t *createKGK(void) {
  boss_t *kgk = createBoss("Konstantinos");
  kgk->teaching = createPassive(kgkQuestions, kgkAnswers, KGK_SIZE);
  return kgk;
}

// takes in the user's input for the answer
char *getAnswer(void) {
  // TODO: get answer from user as input
  printf(">");
}

// function to start the quiz on assembly code
void quiz(boss_t *boss) {
  // PRE: boss->teaching has been initialised
  int correct = 0;
  printf("Wild %s appeared!\n%s starts asking you assembly questions!",
         boss->name, boss->name);
  for (int i = 0; i < boss->teaching->num; i++) {
    printf("Question %d: %s\n", i + 1, boss->teaching->questions[i]);
    const char *input = getAnswer();
    if (strcmp(input, boss->teaching->answers[i]) == 0) {
      correct++;
    }
  }

  printf("You scored %d correct out of %d.\n", correct, MAX_QUESTIONS);
  // if small number of questions correct start turn-based comabat
  if (correct < MIN_QUESTIONS_CORRECT) {
    printf("%s gets aggravated due to your low score...\n", boss->name);
    printf("%s says: mitigations won't save you this time!\n", boss->name);
    // initBattle(boss);
    battle(boss);
  } else {
    printf("%s is happy you scored well!\n", boss->name);
  }
}
