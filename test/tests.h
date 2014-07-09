#ifndef __TESTS_H__
#define __TESTS_H__

#include <string.h>
#include <unistd.h>

#include "action.h"
#include "evaluator.h"
#include "gamestate.h"
#include "timer.h"
#include "pokerai.h"
#include "urlconnection.h"

typedef struct testresult
{
    int failed;
    int numtests;
} TestResult;

TestResult *CreateResult(int failed, int numtests);
void DeleteResult(TestResult *results);

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

/*
 * Test each component of the poker AI
 */
TestResult *TestAction(void);
TestResult *TestEvaluator(void);
TestResult *TestGameState(void);
TestResult *TestTimer(void);
TestResult *TestURLConnection(void);

/*
 * Test the poker AI's logic by creating random games
 * and evaluating its choices.
 * This test should be manually observed to fine-tune
 * the AI's choice thresholds.
 * timeout: how long the AI will simulate games before making a decision
 */
void TestPokerAI(int timeout);

/*
 * Test the poker AI's logic with the specified number of random trials
 * numtrials: the number of hands to play
 */
void TestAILogic(int numtrials);

#endif
