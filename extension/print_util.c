#include "print_util.h"

#define CLEAR ("clear")

void printPreparingGame(void) {

  for (int i = 0; i < 32; i++) {
    system(CLEAR);
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                       Prepare for a new adventure...");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                       ");
    for (int j = 0; j < i; j++) {
      printf("#");
    }
    for (int j = 30; j >= i; j--) {
      printf("-");
    }
    printf("\n");
    fflush(stdout);
    usleep(70000);
  }
  printf("\n");
  printf("\n");
}

void printMenu(bool isInvalid) {
  system(CLEAR);
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("                          Welcome to the ICL Text Adventure!\n\n");
  printf("                          What would you like to do?\n\n");
  if (!isInvalid) {
    usleep(500000);
  }
  fflush(stdout);
  printf("                              [1] NEW GAME\n");
  printf("                              [2] LOAD GAME\n");
  printf("                              [3] QUIT\n");
  printf("\n");
  if (isInvalid) {
    printf("                          You have entered an invalid choice\n");
  } else {
    printf("\n");
  }
  printf("\n");
  printf("                          >> ");
  return;
}

void printInvalid(void) {
  printf("\n                            Invalid choice entered!\n");
  usleep(800000);
}

// funcion to print Room position
void printRoomPosition(room_t *room) {
  switch (room->position) {
  case NORTH:
    printf("North ");
    break;
  case SOUTH:
    printf("South ");
    break;
  case WEST:
    printf("West ");
    break;
  case EAST:
    printf("East ");
    break;
  case CENTRE:
    printf("Central ");
    break;
  }
}

void printItemDetails(item_t **items) {
  printf("                                                           ROOM "
         "ITEMS\n");
  printf("                                                             ");
  for (int i = 0; i < ITEM_NUM; i++) {
    if (items[i]) {
      printf("%s ", items[i]->key);
      printf("\n");
      printf("                                                             ");
    }
  }
}

// function for printing out name of room
void printRoomName(room_t *room1) {

  switch (room1->current_room) {
  case LOBBY:
    printf(" Lobby");
    break;
  case LAB:
    printf("  Lab");
    break;
  case HARRODS:
    printf("Harrods");
    break;
  case FUSION:
    printf("Fusion");
    break;
  case LECTURE_HALL:
    printf("Room 340");
    break;
  }
}

// function for printing out names of adjacent rooms
void printAdjacentRooms(room_t *room1) {
  for (int i = 0; i < 5; i++) {
    if (room1->adjacent_rooms[i]) {
      printRoomPosition(room1->adjacent_rooms[i]);
      printRoomName(room1->adjacent_rooms[i]);
      printf("\n");
      printf("                                                        ");
    }
  }
}

// function purely for testing ouptut of connect RoomName
void printRoomDetails(room_t *room1) {
  printRoomPosition(room1);
  printRoomName(room1);
  printf("Items in the room: ");
  printItemDetails(room1->items);
  printf("number of adjacent rooms: %d\n", room1->adjacent_room_count);
  printf("adjacent rooms:\n");
  printAdjacentRooms(room1);
}

// prints map of current and adjancent room on top right of terminal

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
  for (int i = 0; i < currentState->player->health / 5; i++) {
    printf("#");
  }
  for (int i = 20; i > currentState->player->health / 5; i--) {
    printf("-");
  }

  printf("  %d", currentState->player->health);
}

void printCash(state *currentState) {
  printf("HuxCoins: $%d", currentState->player->cash);
}

void printInventory(state *currentState) {
  printf("                        ");
  int printed = 0;
  for (int i = 0; i < 6; i++) {
    if (currentState->player->inventory[i]) {
      if (printed != 0) {
        printf("|");
      }
      printf(" %s ", currentState->player->inventory[i]->key);
      printed++;
    }
  }
  if (printed == 0) {
    printf("EMPTY INVENTORY");
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

void printStateDetails2(state *state1) {
  system(CLEAR);
  printf("\n");
  //  printf("%s\n", state1->profile.username);
  printHealth(state1);
  printf("                               ");
  if (state1->currentRoom->adjacent_rooms[NORTH]) {
    printRoomPosition(state1->currentRoom->adjacent_rooms[NORTH]);
    printRoomName(state1->currentRoom->adjacent_rooms[NORTH]);
  }
  printf("\n");
  printf("Score:     %d", state1->profile.score);
  printf("                                       ");
  printf("        |\n");
  printCash(state1);
  printf("                                 ");
  if (state1->currentRoom->adjacent_rooms[WEST]) {
    printRoomPosition(state1->currentRoom->adjacent_rooms[WEST]);
    printRoomName(state1->currentRoom->adjacent_rooms[WEST]);
  } else {
    printf("       ");
  }
  printf(" -- ");
  printRoomPosition(state1->currentRoom);
  printRoomName(state1->currentRoom);
  printf(" -- ");
  if (state1->currentRoom->adjacent_rooms[EAST]) {
    printRoomPosition(state1->currentRoom->adjacent_rooms[EAST]);
    printRoomName(state1->currentRoom->adjacent_rooms[EAST]);
    printf("\n");
  } else {
    printf("      \n");
  }
  printf("                                                   ");
  printf("        |\n");
  printf("                                                        ");
  if (state1->currentRoom->adjacent_rooms[SOUTH]) {
    printRoomPosition(state1->currentRoom->adjacent_rooms[SOUTH]);
    printRoomName(state1->currentRoom->adjacent_rooms[SOUTH]);
  } else {
    printf("      \n");
  }
  printf("\n\n\n\n\n\n\n");
  printInventory(state1);
  printf("currently in room: ");
  printRoomDetails(state1->currentRoom);
}

void printStateDetails(state *state1) {
  system(CLEAR);
  printf("\n");
  // printing health
  printHealth(state1);
  printf("                               ");
  // printing current room details
  printRoomPosition(state1->currentRoom);
  printRoomName(state1->currentRoom);
  printf("\n");
  // printing score
  printf("Score:     %d", state1->profile.score);
  printf("                                                   |\n");
  // printing cash
  printCash(state1);
  printf("                                            ");
  // printing adjacent rooms
  printAdjacentRooms(state1->currentRoom);
  printf("\n");
  printItemDetails(state1->currentRoom->items);
  printf("\n\n\n\n\n");
  printInventory(state1);
  printf("\n\n");
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
