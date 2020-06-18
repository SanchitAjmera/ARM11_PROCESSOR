#include "battle.h"
#include "../../src/common/util.h"
#include "../characters/boss/boss.h"
#include "../characters/player/player.h"
#include "battle_constants.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// determines if the game is finished
static bool battleOver(boss_t *boss, player_t *player) {
  return FIGHT(boss)->health <= 0 || player->health <= 0;
}

// determines if the player won
static bool playerWon(boss_t *boss, player_t *player) {
  // PRE: gameover() returns true
  return FIGHT(boss)->health < player->health;
}

static void printAttack(const char *name, int damage, const char *attackName) {
  usleep(100000);
  fflush(stdout);
  printf("%s used '%s'...\n...and dealt %d damage!\n", name, attackName,
         damage);
}

// deals damage to the player
static void attackPlayer(boss_t *boss, player_t *player, int damage,
                         const char *attackName) {
  player->health -= damage;
  printAttack(boss->name, damage, attackName);
}

// deals damage to the boss
static void attackBoss(boss_t *boss, player_t *player, int damage,
                       const char *attackName) {
  if (IS_CRIT(rand())) {
    damage *= CRIT_FACTOR;
  }
  FIGHT(boss) -= damage;
  printAttack("You", damage, attackName);
}

// player's turn in battle
void playerTurn(boss_t *boss, player_t *player) {
  char *com = malloc(sizeof(char) * 30);
  char *arg = malloc(sizeof(char) * 30);
  getCommand(com, arg);
  if (strcmp("attack", com) == 0) {
    attackBoss(boss, player, 10, "your ICL computing powers");
  } else {
    printf("invalid attack");
    return;
  }
}

// boss's turn in battle
static void bossTurn(boss_t *boss, player_t *player) {
  int damage;
  const char *name;
  if (FIGHT(boss)->health < BOSS_LOW_HEALTH(boss)) {
    printf("%s is enraged...\n", boss->name);
    damage = FIGHT(boss)->special;
    name = FIGHT(boss)->specialName;
  } else {
    damage = FIGHT(boss)->attack;
    name = FIGHT(boss)->attackName;
  }
  attackPlayer(boss, player, damage, name);
}

// function to start the battle with the boss
void battle(boss_t *boss, player_t *player) {
  // PRE: boss->fighting has been initialised
  while (!battleOver(boss, player)) {
    playerTurn(boss, player);
    bossTurn(boss, player);
  }
  if (playerWon(boss, player)) {
    // TODO: game over message
  } else {
    // TODO: game over message
  }
}
