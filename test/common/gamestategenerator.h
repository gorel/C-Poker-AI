#ifndef __GAMESTATE_GENERATOR_H__
#define __GAMESTATE_GENERATOR_H__

#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "gamestate.h"

extern const char *gamestate1;
extern const char *gamestate2;
extern const char *gamestate3;
extern const char *gamestate4;

/*
 * Generate a new random game state
 * return: a malloced char * to a new game state
 */
char *GenerateRandomGameState(void);

/*
 * Generates a new game state with only some fields randomized
 * Each field passed in as NULL or zero is given a random value
 * assigned the same way as in GenerateRandomGameState
 * return: a malloced char * to a new game state
 */
char *GenerateGameState(
          char **hand,
          int num_hand,
          char **community,
          int num_community,
          int initialstack,
          int stack,
          int currentbet,
          int callamount,
          int total_players_remaining,
          int oppinitialstack,
          int oppstack,
          int oppcurrentbet);

#endif
