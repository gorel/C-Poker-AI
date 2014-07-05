#include "tests.h"

#define SECOND  1000
#define MINUTE  (SECOND * 60)

const char *gamestate1 = \
"{"
"\"name\": \"TEST_AI\","
"\"your_turn\": \"true\","
"\"initial_stack\": 250,"
"\"stack\": 215,"
"\"current_bet\": 35,"
"\"call_amount\": 0,"
"\"hand\": [\"AH\", \"AD\"],"
"\"community_cards\": [\"AC\"],"
"\"betting_phase\":\"draw\","
"\"players_at_table\": [{"
"   \"player_name\": \"AAA\","
"   \"initial_stack\": 250,"
"   \"current_bet\": 35,"
"   \"stack\": 215,"
"   \"folded\": false"
"   }, {"
"   \"player_name\": \"BBB\","
"   \"initial_stack\": 250,"
"   \"current_bet\": 35,"
"   \"stack\": 215,"
"   \"folded\": false"
"   }],"
"\"total_players_remaining\": 2,"
"\"table_id\": 766,"
"\"round_id\": 823,"
"\"lost_at\": null"
"}";

const char *gamestate2 = \
"{"
"\"name\": \"TEST_AI\","
"\"your_turn\": \"true\","
"\"initial_stack\": 250,"
"\"stack\": 215,"
"\"current_bet\": 35,"
"\"call_amount\": 0,"
"\"hand\": [\"4D\", \"7D\"],"
"\"community_cards\": [\"3D\", \"6D\", \"QD\", \"KD\", \"8D\"],"
"\"betting_phase\":\"draw\","
"\"players_at_table\": [{"
"   \"player_name\": \"AAA\","
"   \"initial_stack\": 250,"
"   \"current_bet\": 35,"
"   \"stack\": 215,"
"   \"folded\": false"
"   }, {"
"   \"player_name\": \"BBB\","
"   \"initial_stack\": 250,"
"   \"current_bet\": 35,"
"   \"stack\": 215,"
"   \"folded\": false"
"   }],"
"\"total_players_remaining\": 2,"
"\"table_id\": 766,"
"\"round_id\": 823,"
"\"lost_at\": null"
"}";

const char *gamestate3 = \
"{"
"\"name\": \"TEST_AI\","
"\"your_turn\": \"true\","
"\"initial_stack\": 250,"
"\"stack\": 215,"
"\"current_bet\": 35,"
"\"call_amount\": 0,"
"\"hand\": [\"2H\", \"7D\"],"
"\"community_cards\": [\"AC\", \"KS\", \"3H\", \"QD\"],"
"\"betting_phase\":\"draw\","
"\"players_at_table\": [{"
"   \"player_name\": \"AAA\","
"   \"initial_stack\": 250,"
"   \"current_bet\": 35,"
"   \"stack\": 215,"
"   \"folded\": false"
"   }, {"
"   \"player_name\": \"BBB\","
"   \"initial_stack\": 250,"
"   \"current_bet\": 35,"
"   \"stack\": 215,"
"   \"folded\": false"
"   }],"
"\"total_players_remaining\": 2,"
"\"table_id\": 766,"
"\"round_id\": 823,"
"\"lost_at\": null"
"}";

const char *gamestate4 = \
"{"
"\"name\": \"TEST_AI\","
"\"your_turn\": \"true\","
"\"initial_stack\": 2500,"
"\"stack\": 2100,"
"\"current_bet\": 400,"
"\"call_amount\": 2000,"
"\"hand\": [\"2H\", \"7D\"],"
"\"community_cards\": [\"AC\", \"KS\", \"3H\", \"QD\", \"AD\"],"
"\"betting_phase\":\"river\","
"\"players_at_table\": [{"
"   \"player_name\": \"AAA\","
"   \"initial_stack\": 2500,"
"   \"current_bet\": 2400,"
"   \"stack\": 100,"
"   \"folded\": false"
"   }, {"
"   \"player_name\": \"BBB\","
"   \"initial_stack\": 6000,"
"   \"current_bet\": 2400,"
"   \"stack\": 3600,"
"   \"folded\": false"
"   }, {"
"   \"player_name\": \"CCC\","
"   \"initial_stack\": 2500,"
"   \"current_bet\": 2400,"
"   \"stack\": 2100,"
"   \"folded\": false"
"   }],"
"\"total_players_remaining\": 3,"
"\"table_id\": 766,"
"\"round_id\": 823,"
"\"lost_at\": null"
"}";

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

void TestPokerAI(int timeout)
{
    cJSON *json = NULL;
    PokerAI *AI = CreatePokerAI(4, timeout);
    FILE *log = fopen("log.txt", "w");
    SetLogging(AI, DEBUG, log);

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

    DestroyPokerAI(AI);
}
