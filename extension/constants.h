#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FLUSH (fflush(stdout))
#define IS_NULL(pointer) (pointer == NULL)

// from game.c
#define SINGLE(type) (type == SKIP || type == HELP || type == EXIT)
#define IS_EMPTY(string) (!strcmp(string, ""))

// from game_util.c
#define ROOM_POSITION_NUMBER (5)
#define TOTAL_ITEM_COUNT (38)
#define TOTAL_BUYABLE_ITEM_COUNT (24)
#define TOTAL_PEAR_COUNT (5)
#define TOTAL_BUYABLE_APPLE_COUNT (5)
#define TOTAL_SHOP_COUNT (10)
#define TOTAL_CASH_COUNT (5)
#define CASH_ITEM_INDEX (4)
#define PEAR_ITEM_INDEX (0)
#define KEYBOARD_ITEM_INDEX (1)
#define MOUSE_ITEM_INDEX (2)
#define MONITOR_ITEM_INDEX (3)
#define PASS_ITEM_INDEX (5)
#define BUYABLE_APPLE_ITEM_INDEX (6)
#define COFFEE_ITEM_INDEX (7)
#define TESCOMD_ITEM_INDEX (8)
#define RUM_ITEM_INDEX (9)
#define BUYABLE_ITEMS_IN_ROOM (5)
#define FUSION_NORTH_INDEX (15)
#define FUSION_WEST_INDEX (16)
#define FUSION_SOUTH_INDEX (17)
#define FUSION_EAST_INDEX (18)
#define HARRODS_START_INDEX (21)
#define BOSSKGK ("konstantinos")
#define BOSSTONY ("tony")

// from game_util.h
#define MAX_ROOM_HISTORY (5)
#define ROOM_COUNT (5)
#define CLEAR ("clear")
#define ITEM_NUM (10)
#define LOOKUP_FAILURE (NULL)
#define INITIAL_CASH (0)
#define MAX_HEALTH (100)
#define FIND_FAIL (-1)
#define REMOVED (NULL)
#define MAX_PROPERTY (8)
#define PROPERTY_NUM (4)
#define USERNAME_CHAR_LIMIT (20)
#define TOTAL_ROOM_COUNT (25)
#define COMMAND_NUM (12)
#define DIR_NUM (4)
#define DESCRIPTION_SIZE (2000)

#endif
