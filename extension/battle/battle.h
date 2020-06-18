#ifndef BATTLE_H
#define BATTLE_H
#include "../characters/boss/boss.h"
#include "../characters/player/player.h"

extern void battle(boss_t *boss, player_t *player, bool correct);
extern void playerTurn(boss_t *boss, player_t *player);

#endif
