#include "tests.h"

#define SECOND      1000
#define MINUTE      (SECOND * 60)

extern char *gamestate1;
extern char *gamestate2;
extern char *gamestate3;
extern char *gamestate4;

/*
 * Print out the hand and community cards from the given json
 * json: the json representing a game state
 */
static
void printcards(cJSON *json)
{
    const char *hand = "hand";
    const char *community = "community_cards";
    cJSON *handjson = cJSON_GetObjectItem(json, hand);
    cJSON *communityjson = cJSON_GetObjectItem(json, community);

    printf("HAND\tCOMMUNITY\n");

    printf("%s", cJSON_GetArrayItem(handjson, 0)->valuestring);
    for (int i = 1; i < cJSON_GetArraySize(handjson); i++)
    {
        printf(" %s", cJSON_GetArrayItem(handjson, i)->valuestring);
    }

    printf("\t");

    printf("%s", cJSON_GetArrayItem(communityjson, 0)->valuestring);
    for (int i = 1; i < cJSON_GetArraySize(communityjson); i++)
    {
        printf(" %s", cJSON_GetArrayItem(communityjson, i)->valuestring);
    }
    printf("\n");
}

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
    PokerAI *AI = CreatePokerAI(4, timeout);
    FILE *log = fopen("log.txt", "w");
    SetLogging(AI, LOGLEVEL_DEBUG, log);

    printf("***GREAT HAND***\n");
    json = cJSON_Parse(gamestate1);
    printcards(json);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***GOOD HAND***\n");
    json = cJSON_Parse(gamestate2);
    printcards(json);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***BAD HAND***\n");
    json = cJSON_Parse(gamestate3);
    printcards(json);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***AWFUL HAND***\n");
    json = cJSON_Parse(gamestate4);
    printcards(json);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    printf("\n");

    printf("***RANDOM HAND***\n");
    gamestate = GenerateGameState();
    json = cJSON_Parse(gamestate);
    printcards(json);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);
    free(gamestate);
    printf("\n");

    DestroyPokerAI(AI);
}
