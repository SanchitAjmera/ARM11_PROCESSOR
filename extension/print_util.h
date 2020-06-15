#ifndef PRINT_UTIL_H
#define PRINT_UTIL_H

#include "game_util.h"

void printRoomName(room_t *room1);
void printAdjacentRooms(room_t *room1);
void printRoomDetails(room_t *room1);
void printBuildingDetails(building_t *huxley);
void printKonstantinos(void);
void printKonstantinosMouthOpen(void);
void printKonstantinosIndent(void);
void printKonstantinosMouthOpenIndent(void);
void printRemaining(void);
void printStateDetails(state *state1);
#endif
