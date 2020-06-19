#ifndef BATTLE_CONSTANTS_H
#define BATTLE_CONSTANTS_H

#define BOSS_LOW_HEALTH(boss) (FIGHT(boss)->maxHealth * 0.25)
#define PLAYER_LOW_HEALTH(player) (MAX_HEALTH * 0.25)
#define CRIT_FACTOR (2)
#define CRIT_CHANCE (8)
#define CRIT_MAX_CHANCE (10)
#define IS_CRIT(num) ((num % CRIT_MAX_CHANCE) > CRIT_CHANCE)
#define PLAYER_CRIT_MSG ("You start typing faster and get a damage buff!\n")
#define PLAYER_ATTACK_NAME ("malicious code")
#define PLAYER_SPECIAL_ATTACK_MSG                                              \
  ("As the deadline draws near you feel more motivated!\n")
#define PLAYER_SPECIAL_NAME ("buffer overflow attack")
#define INVALID_ATTACK_MSG ("invalid attack")
#define BOSS_SPECIAL_ATTACK_MSG ("            %s is enraged...\n")
#define BOSS_DEFEATED_MSG ("            you defeated %s \n")

#endif
