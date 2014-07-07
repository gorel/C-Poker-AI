#include "tests.h"

#define MAXBUF      4096
#define SMALLBUF    128
#define SECOND      1000
#define MINUTE      (SECOND * 60)

char *deck[] = {"2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH", "AH", \
                "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC", "AC", \
                "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD", "AD", \
                "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS", "AS"  };

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

/*
 * Shuffle the global deck array
 * Uses the Fisher-Yates shuffle algorithm
 */
void ShuffleDeck(void)
{
    int size = sizeof(deck) / sizeof(deck[0]);
    for (int i = 1; i < size; i++)
    {
        int swap = rand() % i;

        char *temp = deck[i];
        deck[i] = deck[swap];
        deck[swap] = temp;
    }
}

/*
 * Generate a new random game state
 * return: a malloced char * to a new game state
 */
static
char *GenerateGameState(void)
{
    char state[MAXBUF];
    char hand[MAXBUF];
    char community[MAXBUF];
    char betting_phase[SMALLBUF];
    char cardstr[SMALLBUF];
    char other_players[MAXBUF];
    char playerstr[MAXBUF];
    char *handcards[NUM_HAND];
    char *commcards[NUM_COMMUNITY];

    char *ptr = state;

    ShuffleDeck();
    int deckindex = 0;

    int initialstack = rand() % 9000 + 1000;
    int stack = rand() % initialstack + 250;
    int currentbet = initialstack - stack;
    int call_amount = rand() % stack;
    //Generate 3 to 6 community cards
    int num_community = rand() % 3 + 3;
    int total_players_remaining = rand() % 5 + 1;
    int oppinitialstack;
    int oppstack;
    int oppcurrentbet;

    //Generate hand cards
    handcards[0] = deck[deckindex++];
    handcards[1] = deck[deckindex++];
    sprintf(hand, "\"hand\": [\"%s\", \"%s\"],", handcards[0], handcards[1]);

    //Generate community cards
    sprintf(community, "\"community\": [");
    for (int i = 0; i < num_community - 1; i++)
    {
        commcards[i] = deck[deckindex++];
        sprintf(cardstr, "\"%s\",", commcards[i]);
        strcat(community, cardstr);
    }

    //Generate last community card
    commcards[num_community - 1] = deck[deckindex++];
    sprintf(cardstr, "\"%s\"],", commcards[num_community - 1]);
    strcat(community, cardstr);

    //Generate opponents
    sprintf(other_players, "\"players_at_table\": [{");
    for (int i = 0; i < total_players_remaining - 1; i++)
    {
        oppinitialstack = rand() % 9000 + 1000;
        oppstack = rand() % oppinitialstack + 250;
        oppcurrentbet = oppinitialstack - oppstack;
        sprintf(playerstr, "\'initial_stack\": %d,\"current_bet\": %d,\"stack\": %d,\"folded\": false},{",oppinitialstack, oppstack, oppcurrentbet);
        strcat(other_players, playerstr);

    }

    //Generate last opponent
    oppinitialstack = rand() % 9000 + 1000;
    oppstack = rand() % oppinitialstack + 250;
    oppcurrentbet = oppinitialstack - oppstack;
    sprintf(playerstr, "\'initial_stack\": %d,\"current_bet\": %d,\"stack\": %d,\"folded\": false}],",oppinitialstack, oppstack, oppcurrentbet);
    strcat(other_players, playerstr);

    sprintf(ptr, "{\"name\": \"TEST_AI\",\"your_turn\": \"true\",");
    ptr += strlen("{\"name\": \"TEST_AI\",\"your_turn\": \"true\",");


    sprintf(ptr, "\"table_id\": 123,\"round_id\": 100,\"lost_at\": null}");
    return strdup(state);
}

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

void TestPokerAI(int timeout)
{
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

    DestroyPokerAI(AI);
}
