#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

typedef struct ITEM {

    char* name;
    unsigned short id;

} ITEM;

typedef struct PLAYER {

    char name[255];
    unsigned short health;
    ITEM* inventory;
    unsigned int inv_count;
    unsigned int x;
    unsigned int y;

} PLAYER;

int add_to_inventory(PLAYER* player, ITEM* item);
int remove_from_inventory(PLAYER* player, short id);

#endif
