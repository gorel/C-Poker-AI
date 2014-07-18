#ifndef __POKER_AI_H__
#define __POKER_AI_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "action.h"
#include "evaluator.h"
#include "gamestate.h"
#include "timer.h"

#define NUM_RAISE_LIMIT     2

typedef enum loglevel
{
    LOGLEVEL_NONE,
    LOGLEVEL_INFO,
    LOGLEVEL_DEBUG
} LOGLEVEL;

typedef struct pokerai
{
    //Worker threads
    pthread_mutex_t mutex;
    pthread_t *threads;
    int num_threads;
    int timeout;

    //Scoring
    int games_won;
    int games_simulated;

    //Current game state
    GameState game;
    int num_times_raised;

    //Recommended action
    Action action;

    //Logging for debugging purposes
    LOGLEVEL loglevel;
    FILE *logfile;
} PokerAI;

/*
 * Create a new PokerAI
 *
 * timeout: how long (in milliseconds) each thread may simulate games
 * return a new PokerAI
 */
PokerAI *CreatePokerAI(int timeout);

/*
 * Destroy the PokerAI and all associated memory
 * ai: the PokerAI to destroy
 */
void DestroyPokerAI(PokerAI *ai);

/*
 * Set debug logging to the given level
 * and output to the given FILE
 * ai: the AI to set logging for
 * level: the logging level of output
 * file: the FILE where output should be logged
 */
void SetLogging(PokerAI *ai, LOGLEVEL level, FILE *file);

/*
 * Update the given PokerAI's game state
 * ai: the PokerAI to update
 * new_state: JSON representation of the new game state
 */
void UpdateGameState(PokerAI *ai, cJSON *new_state);

/*
 * Manually set the AI's hand cards
 * ai: the pokerAI to update
 * hand: the cards to give the AI
 * handsize: the number of cards in the AI's hand
 */
void SetHand(PokerAI *ai, char **hand, int handsize);

/*
 * Manually set the AI's community cards
 * ai: the pokerAI to update
 * community: the cards to give the AI
 * communitysize: the number of community cards in play
 */
void SetCommunity(PokerAI *ai, char **community, int communitysize);

/*
 * Return whether or not it is the AI's turn to act
 * ai: the PokerAI to update
 * return: true if it is the AI's turn to act
 */
bool MyTurn(PokerAI *ai);

/*
 * Use Monte Carlo simulation to determine the best action to take
 * ai: the AI that is predicting the best action
 * return: a string representation of the best action to take
 */
char *GetBestAction(PokerAI *ai);

/*
 * Use Monte Carlo simulation to determine the win probability given the AI's hand and community cards
 * ai: the AI that is predicting the win probability
 * return: the win probability as a double in the range [0, 1]
 */
double GetWinProbability(PokerAI *ai);

/*
 * Print the AI's decision to the given FILE
 * ai: the AI that is making the decision
 * file: the FILE where output should be written
 */
void WriteAction(PokerAI *ai, FILE *file);

#endif
