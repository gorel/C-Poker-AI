#include "tests.h"

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

void TestPokerAI(void)
{
    cJSON *json = NULL;
    PokerAI *AI = CreatePokerAI(4, 1000);
    SetLogging(AI, DEBUG, stderr);

    json = cJSON_Parse(gamestate1);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);

    json = cJSON_Parse(gamestate2);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);

    json = cJSON_Parse(gamestate3);
    UpdateGameState(AI, json);
    GetBestAction(AI);
    WriteAction(AI, stdout);
    cJSON_Delete(json);

    DestroyPokerAI(AI);
}
