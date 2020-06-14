#include "boss.h"
#include <stdio.h>

// function to start the battle with the boss
void battle(boss_t *boss, player_t *player) {
  // PRE: boss->fighting has been initialised
  // TODO: turn based combat against the player
}

// determines if the game is finished
bool gameOver(boss_t *boss, player_t *player) {
  return boss->fighting->health <= 0;
  || player->health <= 0;
}

// determins if the player won
bool playerWon(boss_t *boss, player_t *player) {
  // PRE: gamover() returns true
  return boss->fighting->health < player->health;
}

void printAttack(const char *name, int damage, const char *attackName) {
  printf("%s used %s...\n...and dealt %d damage!\n", name, attackName, damage);
}

// deals damage to opponent
void attackPlayer(boss_t *boss, player_t *player) {
  int damage = boss->fighting->attack;
  player->health -= damage;
  printAttack(boss->name, damage, boss->fighting->attackName);
}

// player's turn in battle
void playerTurn(boss_t *boss, player_t *player) {
  // TODO: let them choose a form of attack
}

// boss's turn in battle
void bossTurn(boss_t *boss, player_t *player) {}

void printPlayer() {}

void printBoss() {}
