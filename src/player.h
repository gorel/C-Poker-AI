#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

typedef struct player
{
    char *name;
    int initial_stack;
    int current_bet;
    int stack;
    bool folded;
} Player;

/*
 * Create a new player from the given cJSON object
 * playerjson: a cJSON object representing the player
 * return: a new Player
 */
Player *CreatePlayer(cJSON *playerjson);

/*
 * Destroy the given player
 * player: the player object to free all memory for
 */
void DestroyPlayer(Player *player);

#endif
