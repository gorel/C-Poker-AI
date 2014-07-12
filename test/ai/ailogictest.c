#include "ailogictest.h"

#define LOGFILE     stdout

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
    char *handranksfile = DEFAULT_HANDRANKS_FILE;
    char *gamestate;
    cJSON *json;

    InitEvaluator(handranksfile);
    PokerAI *AI = CreatePokerAI(NUM_CORES, TIMEOUT);
    SetLogging(AI, LOGLEVEL_INFO, LOGFILE);

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
