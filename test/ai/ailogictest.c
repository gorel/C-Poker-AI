#include <math.h>

#include "ailogictest.h"

#define LOGFILENAME "ailogictest.log"
FILE *LOGFILE;

typedef struct stats
{
    int win;
    int loss;
    int unlucky;
} Stats;

struct decision_results
{
    int fold;
    int call;
    int bet;
    Stats fold_stats;
    Stats call_stats;
    Stats bet_stats;
} Results;

typedef enum decision_outcome
{
    DECISION_BAD,
    DECISION_GOOD,
    DECISION_UNLUCKY
} DecisionOutcome;

/*
 * Simulate one more game for the poker AI
 * and update the global struct of its decisions
 * in order to fine tune the logic
 * ai: the poker AI to simulate a game for
 * return: the outcome of the AI's decision
 */
static
DecisionOutcome SimulateLastGame(PokerAI *ai);

int main(int argc, char **argv)
{
    int numtrials = 10;
    LOGFILE = fopen(LOGFILENAME, "w");

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
    int total_score;
    DecisionOutcome outcome;

    Results.fold = 0;
    Results.call = 0;
    Results.bet  = 0;

    Results.fold_stats.win = 0;
    Results.call_stats.win = 0;
    Results.bet_stats.win  = 0;

    Results.fold_stats.loss = 0;
    Results.call_stats.loss = 0;
    Results.bet_stats.loss  = 0;

    Results.fold_stats.unlucky = 0;
    Results.call_stats.unlucky = 0;
    Results.bet_stats.unlucky  = 0;

    InitEvaluator(handranksfile);
    PokerAI *AI = CreatePokerAI(NUM_CORES, TIMEOUT);
    SetLogging(AI, LOGLEVEL_INFO, LOGFILE);

    for (int i = 0; i < numtrials; i++)
    {
        printf("Running test %3d/%3d\n", i, numtrials);

        gamestate = GenerateRandomGameState();
        json = cJSON_Parse(gamestate);
        UpdateGameState(AI, json);
        GetBestAction(AI);

        //We can't judge bluffs effectively
        //since it's hard to test if an opponent would
        //believe the bluff and fold
        if (AI->action.bluff)
        {
            i--;
            continue;
        }

        WriteAction(AI, LOGFILE);
        cJSON_Delete(json);
        free(gamestate);

        //Simulate one more game in order to judge the AI's logic
        outcome = SimulateLastGame(AI);
        switch(outcome)
        {
        case DECISION_BAD:
            fprintf(LOGFILE, "***BAD DECISION***\n\n");
            break;

        case DECISION_GOOD:
            fprintf(LOGFILE, "***GOOD DECISION***\n\n");
            break;
        case DECISION_UNLUCKY:
            fprintf(LOGFILE, "***UNLUCKY DECISION***\n\n");
            break;
        }
    }

    printf("\n\nResults:\n");
    printf("Folding: %10d (%2d good, %2d bad, %2d unlucky)\n", Results.fold, Results.fold_stats.win, Results.fold_stats.loss, Results.fold_stats.unlucky);
    printf("Calling: %10d (%2d good, %2d bad, %2d unlucky)\n", Results.call, Results.call_stats.win, Results.call_stats.loss, Results.call_stats.unlucky);
    printf("Raising: %10d (%2d good, %2d bad, %2d unlucky)\n", Results.bet, Results.bet_stats.win, Results.bet_stats.loss, Results.bet_stats.unlucky);
    total_score = Results.fold + Results.call + Results.bet;

    printf("\nAverage logic score: %6d\n", total_score / numtrials);
}

/*
 * Randomly draw a card from the deck
 * and remove that card from the deck
 * deck: the deck to draw a card from
 * psize: a pointer to the size of the deck
 * return: a random card from the deck
 */
static
int draw(int *deck, int *psize)
{
    int index = rand() % *psize;
    int value = deck[index];
    deck[index] = deck[*psize - 1];
    *psize -= 1;

    return value;
}

/*
 * Calculate the maximum opponent score
 * from the given list of opponent hands
 * opponents: an array of poker hands
 * numopponents: the number of opponents in the array
 * numcards: the size of each hand
 * return: the score of the best hand
 */
static
int BestOpponentHand(int **opponents, int numopponents, int numcards)
{
    int max = 0;
    int score;
    for (int i = 0; i < numopponents; i++)
    {
        score = GetHandValue(opponents[i], numcards);
        if (score > max)
        {
            max = score;
        }
    }

    return max;
}

/*
 * Simulate one more game for the poker AI
 * and update the global struct of its decisions
 * in order to fine tune the logic
 * ai: the poker AI to simulate a game for
 * return: the outcome of the AI's decision
 */
static
DecisionOutcome SimulateLastGame(PokerAI *ai)
{
    GameState *game = &ai->game;
    int me[NUM_HAND + NUM_COMMUNITY];
    int *opponents[MAX_OPPONENTS];
    int community[NUM_COMMUNITY];
    int myscore;
    int bestopponent;

    for (int i = 0; i < game->num_playing; i++)
    {
        opponents[i] = malloc(sizeof(*opponents[i]) * (NUM_HAND + NUM_COMMUNITY));
    }

    //Create a deck as a randomized queue data structure
    int deck[NUM_DECK] = {0};
    int decksize = 0;

    //Cards are 1 indexed
    for (int i = 1; i < NUM_DECK; i++)
    {
        if (game->deck[i])
        {
            deck[decksize] = i;
            decksize++;
        }
    }

    //Add the known community cards to the simulation community list
    for (int i = 0; i < game->communitysize; i++)
    {
        community[i] = game->community[i];
    }

    //Distribute the rest of the community cards
    for (int i = game->communitysize; i < NUM_COMMUNITY; i++)
    {
        community[i] = draw(deck, &decksize);
    }

    //Give each opponent their cards
    for (int opp = 0; opp < game->num_playing; opp++)
    {
        //Personal cards
        for (int i = 0; i < NUM_HAND; i++)
        {
            opponents[opp][i] = draw(deck, &decksize);
        }

        //Community cards
        for (int i = NUM_HAND; i < NUM_COMMUNITY + NUM_HAND; i++)
        {
            opponents[opp][i] = community[i - NUM_HAND];
        }
    }

    //Put my cards into a new array
    for (int i = 0; i < NUM_HAND; i++)
    {
        me[i] = game->hand[i];
    }
    for (int i = NUM_HAND; i < NUM_HAND + NUM_COMMUNITY; i++)
    {
        me[i] = community[i - NUM_HAND];
    }

    //See who won
    myscore = GetHandValue(me, NUM_HAND + NUM_COMMUNITY);
    bestopponent = BestOpponentHand(opponents, game->num_playing, NUM_HAND + NUM_COMMUNITY);

    //Free allocated memory
    for (int i = 0; i < game->num_playing; i++)
    {
        free(opponents[i]);
    }

    switch(ai->action.type)
    {
    case ACTION_FOLD:
        //Folding was a bad decision
        if (myscore > bestopponent)
        {
            Results.fold -= game->current_pot;

            if (ai->action.winprob > 0.45)
            {
                Results.fold_stats.loss++;
            }
            else
            {
                Results.fold_stats.unlucky++;
            }

            return false;
        }
        //Folding saved us from losing more money
        else
        {
            Results.fold += game->call_amount;
            Results.fold_stats.win++;
            return true;
        }
        break;

    case ACTION_CALL:
        //Calling was a good decision
        if (myscore > bestopponent)
        {
            Results.call += game->current_pot;
            Results.call_stats.win++;
            return true;
        }
        //Calling lost us some money
        else
        {
            Results.call -= game->call_amount;

            if (ai->action.winprob < 0.35 && ai->action.expectedgain < 20)
            {
                Results.call_stats.loss++;
            }
            else
            {
                Results.call_stats.unlucky++;
            }

            return false;
        }
        break;

    case ACTION_BET:
        //Betting was a good decision
        if (myscore > bestopponent)
        {
            //Assume half of the players stayed in
            int players_staying = ceil(game->num_playing / 2.0);
            int raise_income = players_staying * ai->action.amount;

            Results.bet += game->current_pot + raise_income;
            Results.bet_stats.win++;
            return true;
        }
        //Raising lost us a lot of money
        else
        {
            Results.bet -= game->call_amount + ai->action.amount;

            if (ai->action.winprob < 0.5)
            {
                Results.bet_stats.loss++;
            }
            else
            {
                Results.bet_stats.unlucky++;
            }
            return false;
        }
        break;

    default:
        fprintf(stderr, "Error: Action unset.\n");
        return false;
    }
}
