#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOM_HISTORY

// enum for items stored by person in inventory
typedef enum { ITEM1, ITEM2, ETC } item;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } room;

// enum for character type
typedef enum { BATMAN, TONY, UTA } character;

typedef struct {
  item *inventory;
  int cash;
  int health;
  room current_room;
  room room_history[MAX_ROOM_HISTORY];
  struct {
    char *username;
    character character;
  } profile;
} state;
