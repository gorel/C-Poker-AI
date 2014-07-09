#include "player.h"

#define JSON(json, field) \
    cJSON_GetObjectItem(json, field)
#define JSON_STRING(json, field) \
    JSON(json, field)->valuestring
#define JSON_INT(json, field) \
    JSON(json, field)->valueint

/*
 * Create a new player from the given cJSON object
 * playerjson: a cJSON object representing the player
 * return: a new Player
 */
Player *CreatePlayer(cJSON *playerjson)
{
    Player *player = malloc(sizeof(Player));

    char *name = JSON_STRING(playerjson, "player_name");
    int initial_stack = JSON_INT(playerjson, "initial_stack");
    int current_bet = JSON_INT(playerjson, "current_bet");
    int stack = JSON_INT(playerjson, "stack");
    bool folded = JSON_INT(playerjson, "folded");

    strncpy(player->name, name, MAX_NAME_LEN);
    player->initial_stack = initial_stack;
    player->current_bet = current_bet;
    player->stack = stack;
    player->folded = folded;

    return player;
}

/*
 * Destroy the given player
 * player: the player object to free all memory for
 */
void DestroyPlayer(Player *player)
{
    if (!player) return;

    free(player);
}
