#ifndef __TESTS_H__
#define __TESTS_H__

#include <string.h>

#include "action.h"
#include "evaluator.h"
#include "gamestate.h"
#include "timer.h"
#include "pokerai.h"
#include "urlconnection.h"

void TestAction(void);
void TestEvaluator(void);
void TestGameState(void);
void TestTimer(void);
void TestURLConnection(void);
void TestPokerAI(int timeout);

#endif
