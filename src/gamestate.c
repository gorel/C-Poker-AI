#include "gamestate.h"

#define JSON(json, field) \
    cJSON_GetObjectItem(json, field)
#define JSON_STRING(json, field) \
    JSON(json, field)->valuestring
#define JSON_INT(json, field) \
    JSON(json, field)->valueint
#define JSON_ARRAY_ELEM(json, i) \
    cJSON_GetArrayItem(json, i)

/*
 * Convert a string phase to the enum phase type
 * phase: the string representation of the current phase
 * return: a Phase representing the current game phase
 */
static
Phase GetPhase(char *phase)
{
    if (!strcmp(phase, "deal"))
    {
        return DEAL;
    }
    else if (!strcmp(phase, "flop"))
    {
        return FLOP;
    }
    else if (!strcmp(phase, "turn"))
    {
        return TURN;
    }
    else if (!strcmp(phase, "river"))
    {
        return RIVER;
    }
    else
    {
        return ERROR;
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
 * Set the card array to the given JSON array
 * cards: the int array of where to place the results
 * json: a JSON array of card strings
 */
static
void GetCardArray(int *cards, cJSON *json)
{
    int i;

    for (i = 0; i < cJSON_GetArraySize(json); i++)
    {
        cards[i] = CardMaskFromString(
                JSON_ARRAY_ELEM(json, i)->valuestring);
    }
}

/*
 * Find the number of opponents who haven't folded yet
 * players: a JSON array of the players at the table
 * return: the number of players who haven't folded yet
 */
static
int GetNumOpponents(cJSON *players)
{
    int i;
    int playing = 0;
    for (i = 0; i < cJSON_GetArraySize(players); i++)
    {
        //TODO: Read json object for this player
    }

    return playing;
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
    game->phase = GetPhase(JSON_STRING(json, "phase"));
    game->your_turn = JSON_INT(json, "your_turn");
    game->num_opponents = GetNumOpponents(JSON(json, "players_at_table"));

    //Set the AI's hand and the community cards
    GetCardArray(game->hand, JSON(json, "hand"));
    GetCardArray(game->community, JSON(json, "community_cards"));
}
