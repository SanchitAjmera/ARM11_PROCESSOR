#include "battle.h"
#include "../characters/boss/boss.h"
#include "../characters/player/player.h"
#include "../emulateARM/emulateARM.h"
#include "../game_util.h"
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

typedef struct {
  char *value;
  int length, maxLength;
} resizable_string;

void extendString(resizable_string *string) {
  if (string->length < string->maxLength - 1) {
    return;
  }
  while (string->length >= string->maxLength - 1) {
    string->maxLength *= 2;
  }
  string->value = realloc(string->value, sizeof(char) * string->maxLength);
}

resizable_string *newString() {
  resizable_string *string = malloc(sizeof(resizable_string));
  string->value = malloc(sizeof(char));
  string->length = 0;
  string->maxLength = 16;
  extendString(string);
  return string;
}

void appendToString(resizable_string *string, const char *append) {
  int oldLength = string->length;
  int appendLength = strlen(append);
  string->length += appendLength;
  extendString(string);
  char *startPos = string->value + oldLength;
  strcpy(startPos, append);
  strcpy(string->value + string->length, "\0");
}

void freeString(resizable_string *string) {
  free(string->value);
  free(string);
}

// player's turn in battle
void playerTurn(boss_t *boss, player_t *player) {
  bool takeInput = true;
  resizable_string *code = newString();

  char *input = malloc(sizeof(char) * 50);
  while (takeInput) {
    printf("                     >> ");
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
  printf("%s\n", code->value);
  char **output = runCode(code->value);
  printOutput(output);

  attackBoss(boss, player, 1, "sanchizzle");

  freeOutput(output);
  freeString(code);
  free(input);
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
