#include "tests.h"

#define TEST_NUM_HAND       2
#define TEST_HAND_STR       "{\"cards\" : [\"2H\", \"7D\"]}"
#define TEST_NUM_COMMUNITY  4
#define TEST_COMMUNITY_STR  "{\"cards\" : [\"AC\", \"KS\", \"3H\", \"QD\"]}"
#define TEST_NUM_OPPONENTS  2
#define TEST_NUM_DECK       NUM_DECK

#define JSON(json, field) \
    cJSON_GetObjectItem(json, field)

int TEST_HAND[TEST_NUM_HAND];
int TEST_COMMUNITY[TEST_NUM_COMMUNITY];
int TEST_DECK[TEST_NUM_DECK];

const char *gamestate = \
"{"
"\"name\": \"TEST_AI\","
"\"your_turn\": true,"
"\"initial_stack\": 250,"
"\"stack\": 215,"
"\"current_bet\": 35,"
"\"call_amount\": 0,"
"\"hand\": [\"2H\", \"7D\"],"
"\"community_cards\": [\"AC\", \"KS\", \"3H\", \"QD\"],"
"\"betting_phase\":\"FLOP\","
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

/*
 * Initialize the arrays used to check the parser
 */
static
void InitializeTestGameState(void);

TestResult *TestGameState(void)
{
    GameState game;
    int numtests = 0;
    int failed = 0;

    InitializeTestGameState();

    cJSON *json = cJSON_Parse(gamestate);
    SetGameState(&game, json);
    cJSON_Delete(json);

    if (game.round_id != 823)
    {
        fprintf(stderr, "Failed round id parse\n");
        failed++;
    }
    numtests++;

    if (game.initial_stack != 250)
    {
        fprintf(stderr, "Failed initial stack parse\n");
        failed++;
    }
    numtests++;

    if (game.stack != 215)
    {
        fprintf(stderr, "Failed stack parse\n");
        failed++;
    }
    numtests++;

    if (game.current_bet != 35)
    {
        fprintf(stderr, "Failed current bet parse\n");
        failed++;
    }
    numtests++;

    if (game.call_amount != 0)
    {
        fprintf(stderr, "Failed call parse\n");
        failed++;
    }
    numtests++;

    if (game.phase != PHASE_FLOP)
    {
        fprintf(stderr, "Failed phase parse\n");
        failed++;
    }
    numtests++;

    for (int i = 0; i < TEST_NUM_HAND; i++)
    {
        if (game.hand[i] != TEST_HAND[i])
        {
            fprintf(stderr, "Failed hand parse\n");
            failed++;
            break;
        }
    }
    numtests++;

    if (game.handsize != NUM_HAND)
    {
        fprintf(stderr, "Failed hand size parse\n");
        failed++;
    }
    numtests++;

    if (game.your_turn != true)
    {
        fprintf(stderr, "Failed your_turn parse\n");
        failed++;
    }
    numtests++;

    if (game.num_opponents != TEST_NUM_OPPONENTS)
    {
        fprintf(stderr, "Failed num_opponents parse\n");
        failed++;
    }
    numtests++;

    for (int i = 0; i < TEST_NUM_COMMUNITY; i++)
    {
        if (game.community[i] != TEST_COMMUNITY[i])
        {
            fprintf(stderr, "Failed community parse\n");
            failed++;
            break;
        }
    }
    numtests++;

    if (game.communitysize != TEST_NUM_COMMUNITY)
    {
        fprintf(stderr, "Failed community size parse\n");
        failed++;
    }
    numtests++;

    //Cards are 1-indexed
    for (int i = 1; i < TEST_NUM_DECK; i++)
    {
        if (game.deck[i] != TEST_DECK[i])
        {
            fprintf(stderr, "Failed deck parse\n");
            failed++;
            break;
        }
    }
    numtests++;

    fprintf(stderr, "[GAMESTATE]\t\tpassed %d/%d\n", (numtests - failed), numtests);
    return CreateResult(failed, numtests);
}

/*
 * Initialize the arrays used to check the parser
 */
static
void InitializeTestGameState(void)
{
    //Cards are 1 indexed
    for (int i = 1; i < TEST_NUM_DECK; i++)
    {
        TEST_DECK[i] = true;
    }

    cJSON *json = cJSON_Parse(TEST_HAND_STR);
    GetCardArray(TEST_HAND, JSON(json, "cards"));
    cJSON_Delete(json);

    json = cJSON_Parse(TEST_COMMUNITY_STR);
    GetCardArray(TEST_COMMUNITY, JSON(json, "cards"));
    cJSON_Delete(json);

    for (int i = 0; i < TEST_NUM_HAND; i++)
    {
        TEST_DECK[TEST_HAND[i]] = false;
    }

    for (int i = 0; i < TEST_NUM_COMMUNITY; i++)
    {
        TEST_DECK[TEST_COMMUNITY[i]] = false;
    }
}
