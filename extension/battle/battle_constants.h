#ifndef BATTLE_CONSTANTS_H
#define BATTLE_CONSTANTS_H

#define BOSS_LOW_HEALTH(boss) (boss->fighting->maxHealth * 0.25)
#define FIGHT(boss) (boss->state->fighting)
#define CRIT_FACTOR (2)
#define CRIT_CHANCE (8)
#define CRIT_MAX_CHANCE (10)
#define IS_CRIT(num) ((num % CRIT_MAX_CHANCE) > CRIT_CHANCE)

#endif
