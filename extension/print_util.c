#include "print_util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CLEAR ("clear")

void printMenu(void) {
  printf("Welcome to the ICL Text Adventure!\n");
  printf("What would you like to do?\n");
  printf("    1. Quit\n");
  printf("    2. Start a new game\n");
  printf("    3. Load a previously saved game\n");
}

// funcion to print Room position
void printRoomPosition(room_t *room) {
  switch (room->position) {
  case NORTH:
    printf("NORTH ");
    break;
  case SOUTH:
    printf("SOUTH ");
    break;
  case WEST:
    printf("WEST ");
    break;
  case EAST:
    printf("EAST ");
    break;
  case CENTRE:
    printf("CENTRE ");
    break;
  }
}

void printItemDetails(item_t **items, int itemCount) {
  if (items != NULL) {
    for (int i = 0; i < itemCount; i++) {
      printf("%s ", items[i]->key);
    }
    printf("\n");
  }
}

// function for printing out name of room
void printRoomName(room_t *room1) {

  switch (room1->current_room) {
  case LOBBY:
    printf("LOBBY\n");
    break;
  case LAB:
    printf("LAB\n");
    break;
  case HARRODS:
    printf("HARRODS\n");
    break;
  case FUSION:
    printf("FUSION\n");
    break;
  case LECTURE_HALL:
    printf("LECTURE_HALL\n");
    break;
  }
}

// function for printing out names of adjacent rooms
void printAdjacentRooms(room_t *room1) {
  for (int i = 0; i < 5; i++) {
    if (room1->adjacent_rooms[i]) {
      printRoomPosition(room1->adjacent_rooms[i]);
      printRoomName(room1->adjacent_rooms[i]);
    }
  }
}

// function purely for testing ouptut of connect RoomName
void printRoomDetails(room_t *room1) {
  printRoomPosition(room1);
  printRoomName(room1);
  printf("Items in the room: ");
  printItemDetails(room1->items, room1->itemCount);
  printf("number of adjacent rooms: %d\n", room1->adjacent_room_count);
  printf("adjacent rooms:\n");
  printAdjacentRooms(room1);
}

bool containsRoom(room_t *printedRooms[4], int size, room_t *room) {
  for (int i = 0; i < size; i++) {
    if (room->current_room == printedRooms[i]->current_room) {
      return true;
    }
  }
  return false;
}

// traverses through all rooms and applies function pointer to each room
void roomTraverser(room_t *entranceRoom, room_t *room1,
                   void (*funcRooms)(room_t *)) {
  if (room1 == NULL) {
    return;
  }
  if (room1->adjacent_rooms != NULL) {
    for (int i = 0; i < 5; i++) {
      if (room1->adjacent_rooms[i]) {
        if (room1->adjacent_rooms[i] != entranceRoom) {
          roomTraverser(room1, room1->adjacent_rooms[i], funcRooms);
        }
      }
    }
  }
  (*funcRooms)(room1);
  printf("\n");
}

// prints details of all rooms in the building
void printBuildingDetails(building_t *huxley) {
  roomTraverser(NULL, huxley->startRoom, printRoomDetails);
}

//--------------------------Start of Player-----------------------------------

void printHealth(state *currentState) {
  printf("Your current health is: %d\n", currentState->player->health);
}

void printCash(state *currentState) {
  printf("You have £%d cash to spend\n", currentState->player->cash);
}

void printInventory(state *currentState) {
  printf("The items in your inventory are:");
  for (int i = 0; i < 6; i++) {
    if (currentState->player->inventory[i]) {
      printf(" %s|", currentState->player->inventory[i]->key);
    }
  }
  printf("\n");
}

char *getPropertyStr(Property property) {
  for (int i = 1; i < PROPERTY_NUM; i++) {
    if (propertyTable[i].value == property) {
      return propertyTable[i].key;
    }
  }
  return LOOKUP_FAILURE;
}

void printProperties(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->player->inventory[item->name]) {
    printf("%s is not in your inventory!\n", itemName);
    return;
  }
  for (int i = 1; i <= MAX_PROPERTY; i <<= 1) {
    if (hasProperty(i, item)) {
      printf(" %s |", getPropertyStr(i));
    }
  }
  printf("\n");
}

void printPlayer(state *currentState) {
  printHealth(currentState);
  printCash(currentState);
  printInventory(currentState);
}

//--------------------------End of Player--------------------------------

void printStateDetails(state *state1) {
  printf("hi I am %s\n", state1->profile.username);
  printf("Score: %d\n", state1->profile.score);
  printHealth(state1);
  printCash(state1);
  printInventory(state1);
  printf("currently in room: ");
  printRoomDetails(state1->currentRoom);
}
// TO BE MOVED:
//-------------------------Konstantinos-----------------------------------------

void printRemaining(void) {
  printf("                                          YOU");
  fflush(stdout);
  printf("       HAVE");
  fflush(stdout);
  printf("       10");
  fflush(stdout);
  printf("       MINUTES");
  fflush(stdout);
  printf("      REMAINING");
  fflush(stdout);
}

void printKonstantinos(void) {
  system(CLEAR);
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                 "
         "_/_/_/_/_/_/_/_/_/_/              "
         "          \n");
  printf("                                         _/_/_/_/_/               "
         "_/_/_/_/_/       "
         "        \n");
  printf("                                   _/_/_/_/_/                        "
         "   _/_/_/_/_/ "
         "     \n");
  printf("                                 _/_/_/_/_/                          "
         "     "
         "_/_/_/_/_/      \n");
  printf("                               _/_/_/_/_/   ####                   "
         "####     "
         "_/_/_/_/_/   \n");
  printf("                               _/_/_/_/_/       ####            #### "
         "       "
         "_/_/_/_/_/ \n");
  printf("                               _/_/_/_/    _______________  "
         "______________    "
         "_/_/_/_/  \n");
  printf("                               _/_/_/_/    _/             ||         "
         "   _/    "
         "_/_/_/_/  \n");
  printf("                               _/_/_/      _/             _/         "
         "   _/      "
         "_/_/_/  \n");
  printf("                               _/_/_/      _/           _/  _/       "
         "   _/      "
         "_/_/_/ \n");
  printf("                               _/_/_/        _/_/_/_/_/_/     "
         "_/_/_/_/_/        "
         "_/_/_/ \n");
  printf("                               _/_/_/_/                 _/  _/       "
         "         "
         "_/_/_/_/ \n");
  printf("                               _/_/_/ _/               _/    _/      "
         "        _/ "
         "_/_/_/ \n");
  printf("                               _/_/_/  _/              _/ _/ _/      "
         "      _/   "
         "_/_/_/ \n");
  printf("                               _/_/      _/                          "
         "    _/       "
         "_/_/ \n");
  printf("                               _/_/       _/                         "
         "   _/        "
         "_/_/ \n");
  printf("                               _/_/        _/      ###############   "
         "  _/         "
         "_/_/ \n");
  printf("                               _/_/          _/       #########      "
         "_/           "
         "_/_/ \n");
  printf("                                _/            _/         _/         "
         "_/             "
         "_/ \n");
  printf("                                _/             _/       _/_/       "
         "_/              "
         "_/ \n");
  printf("                                _/              _/    _/_/_/_/    _/ "
         "              "
         "_/  \n");
  printf("                               _/_/               _/_/_/_/_/_/_/_/   "
         "             "
         "_/_/   \n");
  printf("                              _/_/_/                                 "
         "            "
         "_/_/_/  \n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");

  // fflush(stdout);
  // sleep(3);
}

void printKonstantinosMouthOpen(void) {
  system(CLEAR);
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                 "
         "_/_/_/_/_/_/_/_/_/_/              "
         "          \n");
  printf("                                         _/_/_/_/_/               "
         "_/_/_/_/_/       "
         "        \n");
  printf("                                   _/_/_/_/_/                        "
         "   _/_/_/_/_/ "
         "     \n");
  printf("                                 _/_/_/_/_/                          "
         "     "
         "_/_/_/_/_/      \n");
  printf("                               _/_/_/_/_/   ####                   "
         "####     "
         "_/_/_/_/_/   \n");
  printf("                               _/_/_/_/_/       ####            #### "
         "       "
         "_/_/_/_/_/ \n");
  printf("                               _/_/_/_/    _______________  "
         "______________    "
         "_/_/_/_/  \n");
  printf("                               _/_/_/_/    _/             ||         "
         "   _/    "
         "_/_/_/_/  \n");
  printf("                               _/_/_/      _/             _/         "
         "   _/      "
         "_/_/_/  \n");
  printf("                               _/_/_/      _/           _/  _/       "
         "   _/      "
         "_/_/_/ \n");
  printf("                               _/_/_/        _/_/_/_/_/_/     "
         "_/_/_/_/_/        "
         "_/_/_/ \n");
  printf("                               _/_/_/_/                 _/  _/       "
         "         "
         "_/_/_/_/ \n");
  printf("                               _/_/_/ _/               _/    _/      "
         "        _/ "
         "_/_/_/ \n");
  printf("                               _/_/_/  _/              _/ _/ _/      "
         "      _/   "
         "_/_/_/ \n");
  printf("                               _/_/      _/                          "
         "    _/       "
         "_/_/ \n");
  printf("                               _/_/       _/                         "
         "   _/        "
         "_/_/ \n");
  printf("                               _/_/        _/       #############    "
         "  _/         "
         "_/_/ \n");
  printf("                               _/_/        _/       #           #    "
         "  _/         "
         "_/_/ \n");
  printf("                               _/_/        _/       #           #    "
         "  _/         "
         "_/_/ \n");
  printf("                               _/_/          _/     #############    "
         "_/           "
         "_/_/ \n");
  printf("                                _/            _/         _/         "
         "_/             "
         "_/ \n");
  printf("                                _/             _/       _/_/       "
         "_/              "
         "_/ \n");
  printf("                                _/              _/    _/_/_/_/    _/ "
         "              "
         "_/  \n");
  printf("                               _/_/               _/_/_/_/_/_/_/_/   "
         "             "
         "_/_/   \n");
  printf("                              _/_/_/                                 "
         "            "
         "_/_/_/  \n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
}

void printKonstantinosIndent(void) {
  system(CLEAR);
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                                     "
         "            "
         "_/_/_/_/_/_/_/_/_/_/              "
         "          \n");
  printf("                                                                     "
         "    _/_/_/_/_/               "
         "_/_/_/_/_/       "
         "        \n");
  printf("                                                                   "
         "_/_/_/_/_/                        "
         "   _/_/_/_/_/ "
         "     \n");
  printf("                                                                 "
         "_/_/_/_/_/                          "
         "     "
         "_/_/_/_/_/      \n");
  printf("                                                               "
         "_/_/_/_/_/   ####                   "
         "####     "
         "_/_/_/_/_/   \n");
  printf("                                                               "
         "_/_/_/_/_/       ####            #### "
         "       "
         "_/_/_/_/_/ \n");
  printf("                                                               "
         "_/_/_/_/    _______________  "
         "______________    "
         "_/_/_/_/  \n");
  printf("                                                               "
         "_/_/_/_/    _/             ||         "
         "   _/    "
         "_/_/_/_/  \n");
  printf("                                                               "
         "_/_/_/      _/             _/         "
         "   _/      "
         "_/_/_/  \n");
  printf("                                                               "
         "_/_/_/      _/           _/  _/       "
         "   _/      "
         "_/_/_/ \n");
  printf("                                                               "
         "_/_/_/        _/_/_/_/_/_/     "
         "_/_/_/_/_/        "
         "_/_/_/ \n");
  printf("                                                               "
         "_/_/_/_/                 _/  _/       "
         "         "
         "_/_/_/_/ \n");
  printf("                                                               "
         "_/_/_/ _/               _/    _/      "
         "        _/ "
         "_/_/_/ \n");
  printf("                                                               "
         "_/_/_/  _/              _/ _/ _/      "
         "      _/   "
         "_/_/_/ \n");
  printf("                                                               _/_/  "
         "    _/                          "
         "    _/       "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "     _/                         "
         "   _/        "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "      _/      ###############   "
         "  _/         "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "        _/       #########      "
         "_/           "
         "_/_/ \n");
  printf("                                                                _/   "
         "         _/         _/         "
         "_/             "
         "_/ \n");
  printf("                                                                _/   "
         "          _/       _/_/       "
         "_/              "
         "_/ \n");
  printf("                                                                _/   "
         "           _/    _/_/_/_/    _/ "
         "              "
         "_/  \n");
  printf("                                                               _/_/  "
         "             _/_/_/_/_/_/_/_/   "
         "             "
         "_/_/   \n");
  printf("                                                              _/_/_/ "
         "                                "
         "            "
         "_/_/_/  \n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");

  // fflush(stdout);
  // sleep(3);
}

void printKonstantinosMouthOpenIndent(void) {
  system(CLEAR);
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                               \n");
  printf("                                                                     "
         "            "
         "_/_/_/_/_/_/_/_/_/_/              "
         "          \n");
  printf("                                                                     "
         "    _/_/_/_/_/               "
         "_/_/_/_/_/       "
         "        \n");
  printf("                                                                   "
         "_/_/_/_/_/                        "
         "   _/_/_/_/_/ "
         "     \n");
  printf("                                                                 "
         "_/_/_/_/_/                          "
         "     "
         "_/_/_/_/_/      \n");
  printf("                                                               "
         "_/_/_/_/_/   ####                   "
         "####     "
         "_/_/_/_/_/   \n");
  printf("                                                               "
         "_/_/_/_/_/       ####            #### "
         "       "
         "_/_/_/_/_/ \n");
  printf("                                                               "
         "_/_/_/_/    _______________  "
         "______________    "
         "_/_/_/_/  \n");
  printf("                                                               "
         "_/_/_/_/    _/             ||         "
         "   _/    "
         "_/_/_/_/  \n");
  printf("                                                               "
         "_/_/_/      _/             _/         "
         "   _/      "
         "_/_/_/  \n");
  printf("                                                               "
         "_/_/_/      _/           _/  _/       "
         "   _/      "
         "_/_/_/ \n");
  printf("                                                               "
         "_/_/_/        _/_/_/_/_/_/     "
         "_/_/_/_/_/        "
         "_/_/_/ \n");
  printf("                                                               "
         "_/_/_/_/                 _/  _/       "
         "         "
         "_/_/_/_/ \n");
  printf("                                                               "
         "_/_/_/ _/               _/    _/      "
         "        _/ "
         "_/_/_/ \n");
  printf("                                                               "
         "_/_/_/  _/              _/ _/ _/      "
         "      _/   "
         "_/_/_/ \n");
  printf("                                                               _/_/  "
         "    _/                          "
         "    _/       "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "     _/                         "
         "   _/        "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "      _/       #############    "
         "  _/         "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "      _/       #           #    "
         "  _/         "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "      _/       #           #    "
         "  _/         "
         "_/_/ \n");
  printf("                                                               _/_/  "
         "        _/     #############    "
         "_/           "
         "_/_/ \n");
  printf("                                                                _/   "
         "         _/         _/         "
         "_/             "
         "_/ \n");
  printf("                                                                _/   "
         "          _/       _/_/       "
         "_/              "
         "_/ \n");
  printf("                                                                _/   "
         "           _/    _/_/_/_/    _/ "
         "              "
         "_/  \n");
  printf("                                                               _/_/  "
         "             _/_/_/_/_/_/_/_/   "
         "             "
         "_/_/   \n");
  printf("                                                              _/_/_/ "
         "                                "
         "            "
         "_/_/_/  \n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
}

// function for testing sleep() functions
void printKonstantinosTalking(void) {
  for (int i = 0; i < 18; i++) {
    if (i % 6 == 0 || i % 6 == 1 || i % 6 == 2) {
      printKonstantinos();
      printRemaining();
      fflush(stdout);
      usleep(300000);
      printKonstantinosMouthOpen();
      printRemaining();
      fflush(stdout);
      usleep(300000);
    } else {
      printKonstantinosIndent();
      printRemaining();
      fflush(stdout);
      usleep(300000);
      printKonstantinosMouthOpenIndent();
      printRemaining();
      fflush(stdout);
      usleep(300000);
    }
  }
}

void view(state *currentState, char *argument) {
  if (strcmp(argument, "room") == 0) {
    printRoomDetails(currentState->currentRoom);
  } else if (strcmp(argument, "inventory") == 0) {
    printInventory(currentState);
  }
}
