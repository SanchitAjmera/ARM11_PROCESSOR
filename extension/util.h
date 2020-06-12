#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// enum for items stored by person in inventory
typedef enum { ITEM1, ITEM2, ETC } item;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION } room;

// enum for character type
typedef enum { BATMAN, TONY, UTA } character;

typedef struct {
  item *inventory;
  int cash;
  int health;
  room current_room;
  struct {
    char *username;
    character character;
  } profile;
} person;
