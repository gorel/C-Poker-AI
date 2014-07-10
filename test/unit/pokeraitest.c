#include "tests.h"

#define NUM_CORES   4
#define SECOND      1000
#define MINUTE      (SECOND * 60)

/*
 * Test the poker AI's logic by creating random games
 * and evaluating its choices.
 * This test should be manually observed to fine-tune
 * the AI's choice thresholds.
 * timeout: how long the AI will simulate games before making a decision
 */
void TestPokerAI(int timeout)
{
    char *gamestate;
    cJSON *json = NULL;
    PokerAI *AI = CreatePokerAI(NUM_CORES, timeout);
    FILE *log = stderr;
    SetLogging(AI, LOGLEVEL_INFO, log);

    printf("***GREAT HAND***\n");
    json = cJSON_Parse(gamestate1);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***GOOD HAND***\n");
    json = cJSON_Parse(gamestate2);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***BAD HAND***\n");
    json = cJSON_Parse(gamestate3);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***AWFUL HAND***\n");
    json = cJSON_Parse(gamestate4);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***RANDOM HAND***\n");
    gamestate = GenerateRandomGameState();
    json = cJSON_Parse(gamestate);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    free(gamestate);
    printf("\n");

    DestroyPokerAI(AI);
}
