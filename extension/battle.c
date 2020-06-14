#include "boss.h"
#include <stdio.h>

// function to start the battle with the boss
void battle(boss_t *boss) {
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

// deals damage to opponent
void attack() {}

// player's turn in battle
void playerTurn(boss_t *boss, player_t *player) {}

// boss's turn in battle
void bossTurn(boss_t *boss, player_t *player) {}

void printPlayer() {}

void printBoss() {}
