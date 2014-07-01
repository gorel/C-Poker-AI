#ifndef __POKER_AI_H__
#define __POKER_AI_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "action.h"
#include "evaluator.h"
#include "gamestate.h"
#include "timer.h"

typedef enum loglevel
{
    STANDARD,
    DEBUG
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

    //Recommended action
    Action action;

    //Logging for debugging purposes
    LOGLEVEL loglevel;
    int loggingfd;
} PokerAI;

/*
 * Create a new PokerAI
 *
 * num_threads: number of threads this AI should use
 * timeout: how long (in milliseconds) each thread may simulate games
 * return a new PokerAI
 */
PokerAI *CreatePokerAI(int num_threads, int timeout);

/*
 * Destroy the PokerAI and all associated memory
 * ai: the PokerAI to destroy
 */
void DestroyPokerAI(PokerAI *ai);

/*
 * Set debug logging to the given level
 * and output to the given file descriptor
 * ai: the AI to set logging for
 * level: the logging level of output
 * fd: the file descriptor where output is sent
 */
void SetLogging(PokerAI *ai, LOGLEVEL level, int fd);

/*
 * Update the given PokerAI's game state
 * ai: the PokerAI to update
 * new_state: JSON representation of the new game state
 */
void UpdateGameState(PokerAI *ai, cJSON *new_state);

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
 * Print the AI's decision to the given FILE
 * ai: the AI that is making the decision
 * file: the FILE where output should be written
 */
void WriteAction(PokerAI *ai, FILE *file);

#endif
