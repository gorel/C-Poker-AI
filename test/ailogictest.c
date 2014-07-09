#include "tests.h"

#define NUM_CORES   4
#define TIMEOUT     1000

int main(int argc, char **argv)
{
    int numtrials = 10;
    if (argc == 2)
    {
        numtrials = atoi(argv[1]);
    }

    TestAILogic(numtrials);

    return 0;
}

/*
 * Test the poker AI's logic with the specified number of random trials
 * numtrials: the number of hands to play
 */
void TestAILogic(int numtrials)
{
    char *gamestate;
    cJSON *json;
    PokerAI *AI = CreatePokerAI(NUM_CORES, TIMEOUT);

    for (int i = 0; i < numtrials; i++)
    {
        gamestate = GenerateRandomGameState();
        json = cJSON_Parse(gamestate);
        UpdateGameState(AI, json);
        GetBestAction(AI);
        WriteAction(AI, stdout);
        cJSON_Delete(json);
        free(gamestate);
        printf("\n");

        //TODO: Simulate one more game and "reward" the AI for correct behavior
    }
}
