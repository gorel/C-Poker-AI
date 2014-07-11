#include "gamestategenerator.h"

#define MAXBUF      4096
#define SMALLBUF    128

const char *gamestate1 = \
"{"
"\"name\": \"TEST_AI\","
"\"your_turn\": true,"
"\"initial_stack\": 250,"
"\"stack\": 215,"
"\"current_bet\": 35,"
"\"call_amount\": 0,"
"\"hand\": [\"AH\", \"AD\"],"
"\"community_cards\": [],"
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
"\"your_turn\": true,"
"\"initial_stack\": 250,"
"\"stack\": 215,"
"\"current_bet\": 35,"
"\"call_amount\": 100,"
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
"\"your_turn\": true,"
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
"\"your_turn\": true,"
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
"   \"current_bet\": 400,"
"   \"stack\": 2100,"
"   \"folded\": false"
"   }],"
"\"total_players_remaining\": 3,"
"\"table_id\": 766,"
"\"round_id\": 823,"
"\"lost_at\": null"
"}";

/*
 * Shuffle the given array
 * Uses the Fisher-Yates shuffle algorithm
 * arr: the array to shuffle
 * size: the size of the array
 */
static
void Shuffle(char **arr, int size)
{
    for (int i = 1; i < size; i++)
    {
        int swap = rand() % i;

        char *temp = arr[i];
        arr[i] = arr[swap];
        arr[swap] = temp;
    }
}

/*
 * Remove the given cards from the global deck array
 * This will bring the cards to the deckindex position,
 * then increment deckindex accordingly
 * deck: the deck where cards are being removed from
 * deckindex: the index of where cards will next be drawn
 * cards: the cards to remove from the deck
 * numcards: the number of cards in the cards array
 */
static
void RemoveCardsFromDeck(char **deck, int *deckindex, char **cards, int numcards)
{
    int index = *deckindex;
    int size = sizeof(deck)/sizeof(deck[0]);
    for (int i = index; i < size; i++)
    {
        for (int j = 0; j < numcards; j++)
        {
            if (!strcmp(deck[i], cards[j]))
            {
                char *temp = deck[i];
                deck[i] = deck[index];
                deck[index] = temp;
                index++;
            }
        }
    }
}

/*
 * Generate a new random game state
 * return: a malloced char * to a new game state
 */
char *GenerateRandomGameState(void)
{
    return GenerateGameState(NULL, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

/*
 * Generates a new game state with only some fields randomized
 * Each field passed in as NULL or zero is given a random value
 * assigned the same way as in GenerateRandomGameState
 * return: a malloced char * to a new game state
 */
char *GenerateGameState(
          char **hand,
          int num_hand,
          char **community,
          int num_community,
          int initialstack,
          int stack,
          int currentbet,
          int callamount,
          int total_players_remaining,
          int oppinitialstack,
          int oppstack,
          int oppcurrentbet)
{
    char *deck[] = {"2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH", "AH", \
                "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC", "AC", \
                "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD", "AD", \
                "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS", "AS"  };

    char *names[] = {"Alice", "Bob", "Charlie", "Dave", "Eve", "Freddy", "Gary", "Harry", "Isabel", "Janice", "Kate", "Logan", "Mike", "Oliver", "Pam", "Quinn", "Ryan", "Steve", "Thomas", "Ulysses", "Veronica", "Will", "X-Ray", "Yvette", "Zach"};

    char state[MAXBUF];
    char handstr[MAXBUF];
    char communitystr[MAXBUF];
    char cardstr[SMALLBUF];
    char other_players[MAXBUF];
    char playerstr[MAXBUF];
    char *handcards[NUM_HAND];
    char *commcards[NUM_COMMUNITY];

    int _oppinitialstack;
    int _oppstack;
    int _oppcurrentbet;

    char *ptr = state;

    int decklen = sizeof(deck) / sizeof(deck[0]);
    int nameslen = sizeof(names) / sizeof(names[0]);

    Shuffle(deck, decklen);
    Shuffle(names, nameslen);
    int deckindex = 0;
    int nameindex = 0;

    if (!initialstack)
    {
        initialstack = rand() % 9000 + 1000;
    }

    if (!stack)
    {
        stack = rand() % initialstack + 250;
    }

    if (!currentbet)
    {
        currentbet = initialstack - stack;
    }

    if (!callamount)
    {
        callamount = rand() % stack;
    }

    //Generate 3 to 6 community cards
    if (!num_community)
    {
        num_community = rand() % 3 + 3;
    }

    if (!total_players_remaining)
    {
        total_players_remaining = rand() % 5 + 1;
    }

    //Generate hand cards
    if (!hand)
    {
        handcards[0] = deck[deckindex++];
        handcards[1] = deck[deckindex++];
        sprintf(handstr, "\"hand\": [\"%s\", \"%s\"],\n", handcards[0], handcards[1]);
    }
    else
    {
        RemoveCardsFromDeck(deck, &deckindex, hand, num_hand);
        sprintf(handstr, "\"hand\": [\"%s\", \"%s\"],\n", hand[0], hand[1]);
    }

    //Generate community cards
    if (!community)
    {
        sprintf(communitystr, "\"community_cards\": [");
        for (int i = 0; i < num_community - 1; i++)
        {
            commcards[i] = deck[deckindex++];
            sprintf(cardstr, "\"%s\",", commcards[i]);
            strcat(communitystr, cardstr);
        }

        //Generate last community card
        commcards[num_community - 1] = deck[deckindex++];
        sprintf(cardstr, "\"%s\"],\n", commcards[num_community - 1]);
        strcat(communitystr, cardstr);
    }
    else
    {
        RemoveCardsFromDeck(deck, &deckindex, community, num_community);
        sprintf(communitystr, "\"community_cards\": [");
        for (int i = 0; i < num_community - 1; i++)
        {
            commcards[i] = deck[deckindex++];
            sprintf(cardstr, "\"%s\",", community[i]);
            strcat(communitystr, cardstr);
        }

        sprintf(cardstr, "\"%s\"],\n", community[num_community - 1]);
        strcat(communitystr, cardstr);
    }

    //Generate opponents
    sprintf(other_players, "\"players_at_table\": [\n\t{");
    for (int i = 0; i < total_players_remaining - 1; i++)
    {
        if (!oppinitialstack)
        {
            _oppinitialstack = rand() % 9000 + 1000;
        }
        else
        {
            _oppinitialstack = oppinitialstack;
        }

        if (!oppstack)
        {
            _oppstack = rand() % _oppinitialstack + 250;
        }
        else
        {
            _oppstack = oppstack;
        }

        if (!oppcurrentbet)
        {
            _oppcurrentbet = _oppinitialstack - _oppstack;
        }
        else
        {
            _oppcurrentbet = oppcurrentbet;
        }

        sprintf(playerstr, "   \"player_name\": \"%s\",\"initial_stack\": %d,\"current_bet\": %d,\"stack\": %d,\"folded\": false   },\n\t{", names[nameindex++], _oppinitialstack, _oppstack, _oppcurrentbet);
        strcat(other_players, playerstr);
    }

    //Generate last opponent
    if (!oppinitialstack)
    {
        _oppinitialstack = rand() % 9000 + 1000;
    }
    else
    {
        _oppinitialstack = oppinitialstack;
    }

    if (!oppstack)
    {
        _oppstack = rand() % _oppinitialstack + 250;
    }
    else
    {
        _oppstack = oppstack;
    }

    if (!oppcurrentbet)
    {
        _oppcurrentbet = _oppinitialstack - _oppstack;
    }
    else
    {
        _oppcurrentbet = oppcurrentbet;
    }

    sprintf(playerstr, "   \"player_name\": \"%s\",\"initial_stack\": %d,\"current_bet\": %d,\"stack\": %d,\"folded\": false   }\n\t],\n", names[nameindex++], _oppinitialstack, _oppstack, _oppcurrentbet);
    strcat(other_players, playerstr);

    //////////////////////////////////////
    //Add everything to the state buffer//
    //////////////////////////////////////
    sprintf(ptr, "{\n\t\"name\": \"TEST_AI\",\n\t\"your_turn\": true,\n\t");
    while (*ptr) ptr++;

    sprintf(ptr, "\"initial_stack\": %d,\n\t\"stack\": %d,\n\t\"current_bet\": %d,\n\t", initialstack, stack, currentbet);
    while (*ptr) ptr++;

    sprintf(ptr, "\"call_amount\": %d,\n\t", callamount);
    while (*ptr) ptr++;

    switch(num_community)
    {
    case 3:
        sprintf(ptr, "\"betting_phase\": \"FLOP\",\n\t");
        break;

    case 4:
        sprintf(ptr, "\"betting_phase\": \"TURN\",\n\t");
        break;

    case 5:
        sprintf(ptr, "\"betting_phase\": \"RIVER\",\n\t");
        break;
    }
    while (*ptr) ptr++;

    sprintf(ptr, "%s\t", handstr);
    while (*ptr) ptr++;

    sprintf(ptr, "%s\t", communitystr);
    while (*ptr) ptr++;

    sprintf(ptr, "%s\t", other_players);
    while (*ptr) ptr++;

    sprintf(ptr, "\"total_players_remaining\": %d,\n\t", total_players_remaining);
    while (*ptr) ptr++;

    sprintf(ptr, "\"table_id\": 123,\n\t\"round_id\": 100,\n\t\"lost_at\": null\n}");
    return strdup(state);
}
