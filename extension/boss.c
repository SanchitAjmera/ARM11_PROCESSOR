#include <stdio.h>
#include <stdlib.h>

// dummy function
void validatePtr(void *pointer, const char *) {}

// function to create and initialise a pointer to boss_t on the heap
boss_t *createBoss() {
  boss_t *boss = malloc(sizeof(*boss));
  validatePtr(boss, "Not enough memory.");
  boss->isPassive = false;
  boss->teaching = malloc(sizeof(*boss->teaching));
  validatePtr(boss->teaching, "Not enough memory.");
}

void freeBoss(boss_t *boss) {
  free(boss->teaching);
  free(boss->fighting);
  free(boss);
}

// function to start the quiz on assembly code
void quiz() {}

// function to start the battle with the boos
void battle() {}
