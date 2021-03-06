#include "room_util.h"
#include "../../src/common/util.h"
#include "game_util.h"
#include "item_util.h"

// shows player their inventory of items
void view_inventory() {}

// shows player their character and username
void view_profile() {}

// shows player their current room
void current_room() {}

// shows player room which they can visit
// possibly a map (like the lego land map)
void display_rooms() {}

// generates array of random numbers
void randomiseArray(int randArray[], int length, int randMax) {
  srand(time(NULL));
  for (int i = 0; i < length; i++) {
    randArray[i] = rand() % randMax;
  }
}

// randomly places Items in room
void randomlyPlaceItems(item_t *items[], room_t *rooms[]) {
  int *randomCashLocations = malloc(sizeof(int) * TOTAL_CASH_COUNT);
  checkPtr(randomCashLocations);
  int *randomPearLocations = malloc(sizeof(int) * TOTAL_PEAR_COUNT);
  checkPtr(randomPearLocations);
  checkPtr(randomCashLocations);
  checkPtr(randomPearLocations);
  // array for random locations of apples and cash
  randomiseArray(randomPearLocations, TOTAL_PEAR_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  randomiseArray(randomCashLocations, TOTAL_CASH_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  // dynamically addes apple and cash Items into random rooms
  for (int i = 0; i < 5; i++) {
    rooms[randomCashLocations[i]]
        ->items[rooms[randomCashLocations[i]]->itemCount] = items[i];
    rooms[randomCashLocations[i]]->itemCount++;
    rooms[randomPearLocations[i]]
        ->items[rooms[randomPearLocations[i]]->itemCount] = items[i + 5];
    rooms[randomPearLocations[i]]->itemCount++;
  }
  // 4 IS THE NUMBER OF OTHER ItemS APART FROM CASH & APPLES
  // added other Items randomly around lobby
  int *randomOtherItemLocations = malloc(sizeof(int) * 4);
  checkPtr(randomOtherItemLocations);
  randomiseArray(randomOtherItemLocations, 4, ROOM_POSITION_NUMBER);
  for (int i = 10; i < TOTAL_ITEM_COUNT - TOTAL_BUYABLE_ITEM_COUNT; i++) {
    rooms[randomOtherItemLocations[i - 10]]
        ->items[rooms[randomOtherItemLocations[i - 10]]->itemCount] = items[i];
    rooms[randomOtherItemLocations[i - 10]]->itemCount++;
  }
  free(randomCashLocations);
  free(randomPearLocations);
  free(randomOtherItemLocations);
}

void placeBuyableItems(item_t *items[], room_t *rooms[]) {
  for (int i = 0; i < BUYABLE_ITEMS_IN_ROOM; i++) {
    rooms[FUSION_WEST_INDEX]->items[rooms[FUSION_WEST_INDEX]->itemCount] =
        items[i];
    rooms[FUSION_WEST_INDEX]->itemCount++;

    rooms[FUSION_EAST_INDEX]->items[rooms[FUSION_EAST_INDEX]->itemCount] =
        items[5 + i];
    rooms[FUSION_EAST_INDEX]->itemCount++;

    rooms[FUSION_SOUTH_INDEX]->items[rooms[FUSION_SOUTH_INDEX]->itemCount] =
        items[10 + i];
    rooms[FUSION_SOUTH_INDEX]->itemCount++;

    rooms[FUSION_NORTH_INDEX]->items[rooms[FUSION_NORTH_INDEX]->itemCount] =
        items[15 + i];
    rooms[FUSION_NORTH_INDEX]->itemCount++;
  }
  for (int i = 20; i < TOTAL_ROOM_COUNT - 1; i++) {
    rooms[i]->items[0] = items[i];
    rooms[i]->itemCount++;
  }
}

// connects first room to second room
void connectRoom(room_t *first, room_t *second) {
  switch (first->position) {
  case CENTRE:
    first->adjacent_rooms[second->position] = second;
    second->adjacent_rooms[((second->position) + 2) % 4] = first;
    break;
  // first: north lobby second south fusion
  default:
    first->adjacent_rooms[first->position] = second;
    second->adjacent_rooms[second->position] = first;
    break;
  }
  second->adjacent_room_count++;
  first->adjacent_room_count++;
}

// connects all room positions to center room position r5
void connectRoomPositions(room_t *r1, room_t *r2, room_t *r3, room_t *r4,
                          room_t *r5) {
  room_t *roomArray[ROOM_POSITION_NUMBER - 1] = {r1, r2, r3, r4};
  for (int i = 0; i < ROOM_POSITION_NUMBER - 1; i++) {
    connectRoom(r5, roomArray[i]);
  }
}

// initialises room
room_t *initialiseRoom(RoomName current_room, RoomPosition initial_position) {
  // allocates memory to room and adjacent room array
  room_t *room = malloc(sizeof(*room));
  checkPtr(room);
  room->current_room = current_room;
  room->position = initial_position;
  room->adjacent_room_count = 0;
  room->adjacent_rooms = malloc(sizeof(room_t) * 10);
  checkPtr(room->adjacent_rooms);
  room->itemCount = 0;
  room->items = malloc(sizeof(item_t) * 20);
  checkPtr(room->items);
  room->boss = NULL;
  room->description = NULL;
  return room;
}

void giveRoomDescriptions(room_t *rooms[]) {
  for (int i = 0; i < TOTAL_ROOM_COUNT; i++) {
    rooms[i]->description = strptr(descriptionTable[i]);
  }
}

/* Initialises all rooms in building.
    Takes in a pointer to an array of room_t pointers, and fills this
    array with all the rooms in the building */
building_t *initialiseBuilding(room_t **out) {

  building_t *huxley = malloc(sizeof(*huxley));
  checkPtr(huxley);
  /*
    huxley->startRoom = malloc(sizeof(room_t));
    checkPtr(huxley->startRoom);
  */
  // initialise Items to put in rooms
  // initilaising 5 game apples
  item_t *pear1 = initialiseItem(PEAR_INDEX);
  item_t *pear2 = initialiseItem(PEAR_INDEX);
  item_t *pear3 = initialiseItem(PEAR_INDEX);
  item_t *pear4 = initialiseItem(PEAR_INDEX);
  item_t *pear5 = initialiseItem(PEAR_INDEX);
  // initialising Buyable items
  item_t *buyableItemArray[TOTAL_BUYABLE_ITEM_COUNT];
  initialiseBuyableItem(buyableItemArray);
  // initialising 5 game cash bundles
  item_t *cash1 = initialiseItem(CASH_INDEX);
  item_t *cash2 = initialiseItem(CASH_INDEX);
  item_t *cash3 = initialiseItem(CASH_INDEX);
  item_t *cash4 = initialiseItem(CASH_INDEX);
  item_t *cash5 = initialiseItem(CASH_INDEX);
  // initialising keyboard
  item_t *keyboard = initialiseItem(KEYBOARD_INDEX);
  // initialising mouse
  item_t *mouse = initialiseItem(MOUSE_INDEX);
  // initialing monitor
  item_t *monitor = initialiseItem(MONITOR_INDEX);
  // initialising pass to lab
  item_t *pass = initialiseItem(PASS_INDEX);

  // initialises all the rooms within the building
  room_t *lobbySouth = initialiseRoom(LOBBY, SOUTH);
  room_t *lobbyEast = initialiseRoom(LOBBY, EAST);
  room_t *lobbyWest = initialiseRoom(LOBBY, WEST);
  room_t *lobbyNorth = initialiseRoom(LOBBY, NORTH);
  room_t *lobbyCentre = initialiseRoom(LOBBY, CENTRE);

  room_t *labSouth = initialiseRoom(LAB, SOUTH);
  room_t *labEast = initialiseRoom(LAB, EAST);
  room_t *labWest = initialiseRoom(LAB, WEST);
  room_t *labNorth = initialiseRoom(LAB, NORTH);
  room_t *labCentre = initialiseRoom(LAB, CENTRE);

  room_t *fusionSouth = initialiseRoom(FUSION, SOUTH);
  room_t *fusionEast = initialiseRoom(FUSION, EAST);
  room_t *fusionWest = initialiseRoom(FUSION, WEST);
  room_t *fusionNorth = initialiseRoom(FUSION, NORTH);
  room_t *fusionCentre = initialiseRoom(FUSION, CENTRE);

  room_t *lectureHallSouth = initialiseRoom(LECTURE_HALL, SOUTH);
  room_t *lectureHallEast = initialiseRoom(LECTURE_HALL, EAST);
  room_t *lectureHallWest = initialiseRoom(LECTURE_HALL, WEST);
  room_t *lectureHallNorth = initialiseRoom(LECTURE_HALL, NORTH);
  room_t *lectureHallCentre = initialiseRoom(LECTURE_HALL, CENTRE);

  room_t *harrodsSouth = initialiseRoom(HARRODS, SOUTH);
  room_t *harrodsEast = initialiseRoom(HARRODS, EAST);
  room_t *harrodsWest = initialiseRoom(HARRODS, WEST);
  room_t *harrodsNorth = initialiseRoom(HARRODS, NORTH);
  room_t *harrodsCentre = initialiseRoom(HARRODS, CENTRE);

  // connecting room positions with centre of rooms
  connectRoomPositions(lobbySouth, lobbyEast, lobbyWest, lobbyNorth,
                       lobbyCentre);
  connectRoomPositions(labSouth, labEast, labWest, labNorth, labCentre);
  connectRoomPositions(lectureHallSouth, lectureHallEast, lectureHallWest,
                       lectureHallNorth, lectureHallCentre);
  connectRoomPositions(fusionSouth, fusionEast, fusionWest, fusionNorth,
                       fusionCentre);
  connectRoomPositions(harrodsSouth, harrodsEast, harrodsWest, harrodsNorth,
                       harrodsCentre);

  // connecting rooms which link to each other
  connectRoom(lobbyWest, labEast);
  connectRoom(lobbyNorth, lectureHallSouth);
  connectRoom(lobbyEast, fusionWest);
  connectRoom(fusionNorth, harrodsSouth);

  // initialising Bosses
  boss_t *kgk = createBoss(BOSSKGK);
  boss_t *tony = createBoss(BOSSTONY);

  // adding bosses to Rooms
  labCentre->boss = kgk;
  lectureHallCentre->boss = tony;
  // Keep it in this order I beg
  room_t *roomArray[TOTAL_ROOM_COUNT] = {
      lobbySouth,       lobbyEast,       lobbyNorth,        lobbyWest,
      lobbyCentre,      labSouth,        labEast,           labNorth,
      labWest,          labCentre,       lectureHallSouth,  lectureHallEast,
      lectureHallNorth, lectureHallWest, lectureHallCentre, fusionSouth,
      fusionEast,       fusionNorth,     fusionWest,        fusionCentre,
      harrodsSouth,     harrodsEast,     harrodsNorth,      harrodsWest,
      harrodsCentre};
  for (int i = 0; i < TOTAL_ROOM_COUNT; i++) {
    out[i] = roomArray[i];
  }
  // assigning DESCRIPTIONs
  giveRoomDescriptions(roomArray);

  item_t *itemArray[TOTAL_ITEM_COUNT - TOTAL_BUYABLE_ITEM_COUNT] = {
      cash1, cash2, cash3, cash4,    cash5, pear1,   pear2,
      pear3, pear4, pear5, keyboard, mouse, monitor, pass};

  randomlyPlaceItems(itemArray, roomArray);
  placeBuyableItems(buyableItemArray, roomArray);
  huxley->startRoom = lobbySouth;
  return huxley;
}

// frees room
void freeRoom(room_t *entranceRoom, room_t *room1) {
  if (IS_NULL(room1)) {
    return;
  }

  if (room1->adjacent_rooms != NULL) {
    for (int i = 0; i < room1->adjacent_room_count; i++) {
      // check to stop function from repeatedly freeing the same rooms
      if (room1->adjacent_rooms[i] != entranceRoom) {
        freeRoom(room1, room1->adjacent_rooms[i]);
      }
    }
    free(room1->adjacent_rooms);
  }

  if (room1->items != NULL) {
    for (int i = 0; i < room1->itemCount; i++) {
      free(room1->items[i]);
    }
    free(room1->items);
  }

  if (room1->description != NULL) {
    free(room1->description);
  }
  free(room1);
}

// frees all the rooms in the building
void freeBuilding(building_t *huxley) {
  if (IS_NULL(huxley)) {
    return;
  }
  // calling recursive function to traverse through and free all the rooms
  freeRoom(NULL, huxley->startRoom);
  free(huxley);
}
