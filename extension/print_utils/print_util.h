#ifndef PRINT_UTIL_H
#define PRINT_UTIL_H

#include "../game_utils/game_util.h"

extern void printRoomName(room_t *room1);
extern void printAdjacentRooms(room_t *room1);
extern void printRoomDetails(room_t *room1);
extern void printItemDetails(item_t **items);
extern void printBuildingDetails(building_t *huxley);
extern void printKonstantinosTalking(void);
extern void printRemaining(void);
extern void printStateDetails(state *state1);
extern void printMenu(bool isValid);
extern void printInventory(state *currentState);
extern void view(state *currentState, char *argument);
extern void printPreparingGame(void);
extern void printInvalid(void);
extern void printMap(state *currentState);
extern char *getPropertyStr(Property property);
extern void printCash(state *currentState);
extern void printHealth(state *currentState);
extern void printRoomPosition(room_t *room);
#endif
