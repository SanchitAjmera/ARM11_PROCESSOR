#ifndef PRINT_UTIL_H
#define PRINT_UTIL_H

#include "game_util.h"

extern void printRoomName(room_t *room1);
extern void printAdjacentRooms(room_t *room1);
extern void printRoomDetails(room_t *room1);
extern void printItemDetails(item_t **items);
extern void printBuildingDetails(building_t *huxley);
extern void printKonstantinos(void);
extern void printKonstantinosMouthOpen(void);
extern void printKonstantinosIndent(void);
extern void printKonstantinosMouthOpenIndent(void);
extern void printRemaining(void);
extern void printStateDetails(state *state1);
extern void printMenu(bool isValid);
extern void printInventory(state *currentState);
extern void view(state *currentState, char *argument);
extern void printPreparingGame(void);
extern void printInvalid(void);
extern void printMap(state *currentState);

#endif
