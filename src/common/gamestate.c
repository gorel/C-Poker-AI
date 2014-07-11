#include "gamestate.h"

#define JSON(json, field) \
    cJSON_GetObjectItem(json, field)
#define JSON_STRING(json, field) \
    JSON(json, field)->valuestring
#define JSON_INT(json, field) \
    JSON(json, field)->valueint
#define JSON_ARRAY_ELEM(json, i) \
    cJSON_GetArrayItem(json, i)

//Maps an integer to a card string
char *CARDS[] = {"XX",                                                  \
"2S", "2C", "2D", "2H", "3S", "3C", "3D", "3H", "4S", "4C", "4D", "4H", \
"5S", "5C", "5D", "5H", "6S", "6C", "6D", "6H", "7S", "7C", "7D", "7H", \
"8S", "8C", "8D", "8H", "9S", "9C", "9D", "9H", "TS", "TC", "TD", "TH", \
"JS", "JC", "JD", "JH", "QS", "QC", "QD", "QH", "KS", "KC", "KD", "KH", \
"AS", "AC", "AD", "AH"                                                  \
};

/*
 * Convert a string phase to the enum phase type
 * phase: the string representation of the current phase
 * return: a Phase representing the current game phase
 */
static
Phase GetPhase(char *phase)
{
    if (!strcasecmp(phase, "deal"))
    {
        return PHASE_DEAL;
    }
    else if (!strcasecmp(phase, "flop"))
    {
        return PHASE_FLOP;
    }
    else if (!strcasecmp(phase, "turn"))
    {
        return PHASE_TURN;
    }
    else if (!strcasecmp(phase, "river"))
    {
        return PHASE_RIVER;
    }
    else
    {
        return PHASE_ERROR;
    }
}

/*
 * Create a mask representing the given card
 * card: the string representation of the card
 * return: the representation of this card as an int
 */
static
int CardMaskFromString(char *card)
{
    int val;
    char c = card[0];
    if (c >= '2' && c <= '9')
    {
        val = 4 * (c - '2') + 1;
    }
    else if (c == 'T')
    {
        val = 33;
    }
    else if (c == 'J')
    {
        val = 37;
    }
    else if (c == 'Q')
    {
        val = 41;
    }
    else if (c == 'K')
    {
        val = 45;
    }
    else // c == 'A'
    {
        val = 49;
    }

    c = card[1];
    if (c == 'C')
    {
        val += 1;
    }
    else if (c == 'D')
    {
        val += 2;
    }
    else if (c == 'H')
    {
        val += 3;
    }
    //if c == 'S', val += 0

    return val;
}

/*
 * Remove cards from the deck that have already been seen in play
 * deck: the deck to remove cards from
 * cards: the cards to remove
 * numcards: the number of cards to remove
 */
static inline
void RemoveCardsFromDeck(bool *deck, int *cards, int numcards)
{
    for (int i = 0; i < numcards; i++)
    {
        deck[cards[i]] = false;
    }
}

/*
 * Set the opponents for the game
 * game: the gamestate to set the opponents for
 * players: cJSON representing the players at the table
 */
static
void SetGameOpponents(GameState *game, cJSON *players)
{
    int playing = 0;
    char *name;
    int initial_stack;
    int stack;
    int current_bet;
    bool folded;

    for (int i = 0; i < cJSON_GetArraySize(players) && i < MAX_OPPONENTS; i++)
    {
        cJSON *player = JSON_ARRAY_ELEM(players, i);
        name = JSON_STRING(player, "player_name");
        initial_stack = JSON_INT(player, "initial_stack");
        stack = JSON_INT(player, "stack");
        current_bet = JSON_INT(player, "current_bet");
        folded = JSON_INT(player, "folded");

        strncpy(game->opponents[i].name, name, MAX_NAME_LEN);
        game->opponents[i].initial_stack = initial_stack;
        game->opponents[i].stack = stack;
        game->opponents[i].current_bet = current_bet;
        game->opponents[i].folded = folded;

        if (!folded)
        {
            playing++;
        }
    }

    game->num_opponents = cJSON_GetArraySize(players);
    game->num_playing = playing;
}

/*
 * Set the card array to the given JSON array
 * cards: the int array of where to place the results
 * json: a JSON array of card strings
 * return: the number of cards in the array
 */
int GetCardArray(int *cards, cJSON *json)
{
    int i;

    for (i = 0; i < cJSON_GetArraySize(json); i++)
    {
        cards[i] = CardMaskFromString(
                JSON_ARRAY_ELEM(json, i)->valuestring);
    }

    return cJSON_GetArraySize(json);
}

/*
 * Set the game state according to the given JSON
 * game: the game state to set
 * json: a JSON representation of the game state
 */
void SetGameState(GameState *game, cJSON *json)
{
    game->round_id = JSON_INT(json, "round_id");
    game->initial_stack = JSON_INT(json, "initial_stack");
    game->stack = JSON_INT(json, "stack");
    game->current_bet = JSON_INT(json, "current_bet");
    game->call_amount = JSON_INT(json, "call_amount");
    game->phase = GetPhase(JSON_STRING(json, "betting_phase"));
    game->your_turn = JSON_INT(json, "your_turn");

    //Set the AI's list of opponents
    SetGameOpponents(game, JSON(json, "players_at_table"));

    //Update the current pot
    game->current_pot = game->current_bet;
    for (int i = 0; i < game->num_opponents; i++)
    {
        game->current_pot += game->opponents[i].current_bet;
    }

    //Set the AI's hand and the community cards
    game->handsize = GetCardArray(game->hand, JSON(json, "hand"));
    game->communitysize = GetCardArray(game->community, JSON(json, "community_cards"));

    //Remove these cards from the deck
    memset(game->deck, 1, sizeof(game->deck) / sizeof(game->deck[0]));
    RemoveCardsFromDeck(game->deck, game->hand, game->handsize);
    RemoveCardsFromDeck(game->deck, game->community, game->communitysize);
}

/*
 * Print the current table information
 * game: the game state containing the table information
 * logfile: the file for logging output
 */
void PrintTableInfo(GameState *game, FILE *logfile)
{
    PrintCards(game, logfile);
    fprintf(logfile, "Current pot: %5d\n", game->current_pot);
    PrintPlayers(game, logfile);
}

/*
 * Print the AI's cards to the given file
 * game: the game state containing the cards
 * logfile: the file for logging output
 */
void PrintCards(GameState *game, FILE *logfile)
{
    fprintf(logfile, "Hand\tCommunity\n");

    //Print the hand
    if (game->handsize > 0)
    {
        fprintf(logfile, "%s", CARDS[game->hand[0]]);
        for (int i = 1; i < game->handsize; i++)
        {
            fprintf(logfile, " %s", CARDS[game->hand[i]]);
        }
    }
    else
    {
        fprintf(logfile, "\t");
    }
    fprintf(logfile, "\t");

    //Print the community cards
    if (game->communitysize > 0)
    {
        fprintf(logfile, "%s", CARDS[game->community[0]]);
        for (int i = 1; i < game->communitysize; i++)
        {
            fprintf(logfile, " %s", CARDS[game->community[i]]);
        }
    }
    fprintf(logfile, "\n");

}

/*
 * Print the current players at the table and their information
 * game: the game state containing the players
 * logfile: the file for logging output
 */
void PrintPlayers(GameState *game, FILE *logfile)
{
    Player *player;

    fprintf(logfile, "Me:\n");
    fprintf(logfile, "\tinitial_stack=%5d, call_amount=%5d\n", game->initial_stack, game->call_amount);
    fprintf(logfile, "\t        stack=%5d, current_bet=%5d\n", game->stack, game->current_bet);

    fprintf(logfile, "Opponents:\n");
    for (int i = 0; i < game->num_opponents; i++)
    {
        player = &game->opponents[i];
        if (player->folded)
        {
            fprintf(logfile, "[FOLDED] %10s: initial_stack=%5d, stack=%5d, current_bet=%5d\n", player->name, player->initial_stack, player->stack, player->current_bet);
        }
        else
        {
            fprintf(logfile, "%10s: initial_stack=%5d, stack=%5d, current_bet=%5d\n", player->name, player->initial_stack, player->stack, player->current_bet);
        }
    }
}
