#include "boss.h"
#include <stdio.h>

// determines if the game is finished
bool gameOver(boss_t *boss, player_t *player) {
  return FIGHT(boss)->health <= 0;
  || player->health <= 0;
}

// determins if the player won
bool playerWon(boss_t *boss, player_t *player) {
  // PRE: gamover() returns true
  return FIGHT(boss)->health < player->health;
}

void printAttack(const char *name, int damage, const char *attackName) {
  printf("%s used %s...\n...and dealt %d damage!\n", name, attackName, damage);
}

// deals damage to opponent
void attackPlayer(boss_t *boss, player_t *player, int damage,
                  const char *attackName) {
  player->health -= damage;
  printAttack(boss->name, damage, attackName);
}

// to display battle options to user
void printPlayer(player_t *player) {
  // // TODO: @Luqman to code...
}

// player's turn in battle
void playerTurn(boss_t *boss, player_t *player) {}

// boss's turn in battle
void bossTurn(boss_t *boss, player_t *player) {
  if (FIGHT(boss)->health < BOSS_LOW_HEALTH(boss)) {
    attackPlayer(boss, player, FIGHT(boss)->special, FIGHT(boss)->specialName);
    return;
  }
  attackPlayer(boss, player, FIGHT(boss)->attack, FIGHT(boss)->attackName);
}

// function to start the battle with the boss
void battle(boss_t *boss, player_t *player) {
  // PRE: boss->fighting has been initialised
  // TODO: turn based combat against the player
  while (!gameOver(boss, player)) {
    playerTurn(boss, player);
    bossTurn(boss, player);
  }
  playerWon(boss, player);
}
