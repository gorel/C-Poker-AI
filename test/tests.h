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
char *GenerateGameState(void);

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

#endif
