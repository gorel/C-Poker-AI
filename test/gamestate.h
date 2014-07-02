#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include <stdbool.h>
#include <string.h>

#include "cJSON.h"

#define NUM_HAND        2
#define NUM_COMMUNITY   5
#define NUM_DECK        52
#define MAX_OPPONENTS   10

typedef enum phase
{
    DEAL,
    FLOP,
    TURN,
    RIVER,
    ERROR
} Phase;

typedef struct gamestate
{
    int round_id;
    int initial_stack;
    int stack;
    int current_bet;
    int call_amount;
    Phase phase;
    int hand[NUM_HAND];
    int handsize;
    bool your_turn;
    int num_opponents;
    int community[NUM_COMMUNITY];
    int communitysize;
    //Cards are 1 indexed
    bool deck[NUM_DECK + 1];
} GameState;

/*
 * Set the game state according to the given JSON
 * game: the game state to set
 * json: a JSON representation of the game state
 */
void SetGameState(GameState *game, cJSON *json);

#endif
