#include <stdio.h>
#include <stdlib.h>

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
  boss->fighting = malloc(sizeof(*boss->fighting));
  *boss->fighting = stats;
}

boss_t *createKGK(void) {
  boss_t *kgk = createBoss("Konstantinos");
  kgk->teaching = createPassive(kgkQuestions, kgkAnswers, KGK_SIZE);
  return kgk;
}

// function to start the quiz on assembly code
void quiz(boss_t *boss) {
  // PRE: boss->teaching has been initialised
  // TODO: WHILE loop asking questions
  int correct = 0;
  for (int i = 0; i < boss->teaching->num; i++) {
    // TODO: get answer from user as input
    // char *answer = askQuestion(boss->teaching->questions[i]);
    // TODO: check if answer provided is correct
    // IF checkAnswer(boss, answer);
    // THEN correct++;
  }

  // if small number of questions correct start turn-based comabat
  if (correct < MIN_QUESTIONS_CORRECT) {
    battle(boss);
  }
}
