#ifndef __AI_LOGIC_TEST_H__
#define __AI_LOGIC_TEST_H__

#include "gamestategenerator.h"
#include "pokerai.h"

#define NUM_CORES   4
#define TIMEOUT     1000

/*
 * Test the poker AI's logic with the specified number of random trials
 * numtrials: the number of hands to play
 */
void TestAILogic(int numtrials);

#endif
