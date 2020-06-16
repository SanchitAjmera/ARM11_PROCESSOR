#include "boss.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void printTestName(const char *function) {
  printf("Test name: %s\n", function);
}

void printTestFunc(const char *function) {
  printf("Testing function: %s\n", function);
}

bool testInitBossPtr(void) {
  printTestName("testInitBoss");
  boss_t *boss = initBoss("Alex");
  return boss != NULL;
}

bool testInitBossName(void) {
  printTestName("testInitBossPtr");
  boss_t *boss = initBoss("Alex");
  return strcmp(boss->name, "Alex") == 0;
}

void testInitBossAll(void) {
  printTestName("initBoss");
  assert(testInitBossPtr());
  assert(testInitBossName());
}

bool testCreateBossFail(void) {
  printTestName("testCreateBossKGK");
  boss_t *boss = createBoss("Loqmaan");
  return boss == NULL;
}

bool testCreateBossKGK(void) {
  printTestName("testCreateBossKGK");
  boss_t *kgk = createBoss("Konstantinos");
  return kgk != NULL;
}

bool testCreateBossKGKTeaching(void) {
  printTestName("testCreateBossKGKTeaching");
  boss_t *kgk = createBoss("Konstantinos");
  return kgk->state->teaching != NULL;
}

bool testCreateBossKGKTeachingQs(void) {
  printTestName("testCreateBossKGKTeachingQs");
  boss_t *kgk = createBoss("Konstantinos");
  return kgk->state->teaching->questions == kgkQuestions;
}

bool testCreateBossKGKTeachingAs(void) {
  printTestName("testCreateBossKGKTeachingAs");
  boss_t *kgk = createBoss("Konstantinos");
  return kgk->state->teaching->questions == kgkAnswers;
}

void testCreateBossAll(void) {
  printTestFunc("testCreateBoss");
  assert(testCreateBossFail());
  assert(testCreateBossKGK());
  assert(testCreateBossKGKTeaching());
  assert(testCreateBossKGKTeachingQs());
  assert(testCreateBossKGKTeachingAs());
}

int main(void) {
  testInitBossAll();
  testCreateBossAll();
  return EXIT_SUCCESS;
}
