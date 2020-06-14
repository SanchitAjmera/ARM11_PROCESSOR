#include <stdio.h>
#include <stdlib.h>

// dummy function
void validatePtr(void *pointer, const char *) {}

passive_t *createPassive(const char **questions, const char **answers) {
  passive_t *passive = malloc(sizeof(*passive));
  validatePtr(boss, "Not enough memory.");
  passive->questions = questions;
  passive->answers = answers;
  return passive;
}

// function to create and initialise a pointer to boss_t on the heap
boss_t *createBoss(const char *name) {
  boss_t *boss = malloc(sizeof(*boss));
  validatePtr(boss, "Not enough memory.");
  boss->name = malloc(sizeof(*boss->name));
  validatePtr(boss->name, "Not enough memory.");
  strcpy(boss->name, name);
  boss->isPassive = true;
  boss->teaching = malloc(sizeof(*boss->teaching));
  validatePtr(boss->teaching, "Not enough memory.");
}

void freeBossFighting(aggressive_t *aggressive) {
  free(attackName);
  free(specialName);
  free(aggressive);
}

void freeBossTeaching(passive_t *passive) {
  free(questions);
  free(answers);
  free(passive);
}

void freeBoss(boss_t *boss) {
  freeBossTeaching(boss->teaching);
  freeBossFighting(boss->fighting);
  free(boss->name);
  free(boss);
}

boss_t *createKGK(void) {
  boss_t *kgk = createBoss("Konstantinos");
  kgk->teaching->questions = kgkQuestions;
  kgk->teaching->answers = kgkAnswers;
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
