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

TestResult *TestAction(void);
TestResult *TestEvaluator(void);
TestResult *TestGameState(void);
TestResult *TestTimer(void);
TestResult *TestURLConnection(void);
TestResult *TestPokerAI(int timeout);

#endif
