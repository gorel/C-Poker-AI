#include "pokerai.h"

//Naive way to get thread ID
#define THREAD_ID ((unsigned int)pthread_self() % 100)

/*
 * Calculate the preflop win probability based on hole cards
 * hand: the AI's hole cards
 * return: the win probability of the given hole cards
 */
static
double PreflopWinProbability(int *hand);

/*
 * Spawn Monte Carlo threads to simulate poker games
 * ai: the AI which should spawn the threads
 */
static
void SpawnMonteCarloThreads(PokerAI *ai);

/*
 * Simulate games for the given AI
 * _ai: a void pointer to a PokerAI pointer
 * return: NULL (pthread requirement)
 */
static
void *SimulateGames(void *_ai);

/*
 * Simulate a single poker game for the given AI
 * ai: the poker AI to simulate games for
 * return: 1 on AI win, 0 on AI lose
 */
static
int SimulateSingleGame(PokerAI *ai);

/*
 * Randomly draw a card from the deck
 * and remove that card from the deck
 * deck: the deck to draw a card from
 * psize: a pointer to the size of the deck
 * return: a random card from the deck
 */
static
int draw(int *deck, int *psize);

/*
 * Calculate the maximum opponent score
 * from the given list of opponent hands
 * opponents: an array of poker hands
 * numopponents: the number of opponents in the array
 * numcards: the size of each hand
 * return: the score of the best hand
 */
static
int BestOpponentHand(int **opponents, int numopponents, int numcards);

/*
 * Set the AI's action given its expected gain
 * ai: the AI to set the action for
 */
static
void MakeDecision(PokerAI *ai);

/*
 * Create a new PokerAI
 *
 * num_threads: number of threads this AI should use
 * timeout: how long (in milliseconds) each thread may simulate games
 * return a new PokerAI
 */
PokerAI *CreatePokerAI(int num_threads, int timeout)
{
    PokerAI *ai = malloc(sizeof(PokerAI));

    //Allocate worker thread members
    ai->num_threads = num_threads;
    ai->timeout = timeout;
    ai->threads = malloc(sizeof(pthread_t) * num_threads);
    pthread_mutex_init(&ai->mutex, NULL);

    //Set the initial state to no other players
    ai->game.num_opponents = 0;
    ai->game.num_playing = 0;

    ai->logfile = NULL;
    return ai;
}

/*
 * Destroy the PokerAI and all associated memory
 * ai: the PokerAI to destroy
 */
void DestroyPokerAI(PokerAI *ai)
{
    if (!ai) return;

    if (ai->logfile)
    {
        fclose(ai->logfile);
    }

    pthread_mutex_destroy(&ai->mutex);
    free(ai->threads);
    free(ai);
}

/*
 * Set debug logging to the given level
 * and output to the given FILE
 * ai: the AI to set logging for
 * level: the logging level of output
 * file: the FILE where output should be logged
 */
void SetLogging(PokerAI *ai, LOGLEVEL level, FILE *file)
{
    ai->loglevel = level;
    ai->logfile = file;
}

/*
 * Update the given PokerAI's game state
 * ai: the PokerAI to update
 * new_state: JSON representation of the new game state
 */
void UpdateGameState(PokerAI *ai, cJSON *new_state)
{
    ai->action.type = ACTION_UNSET;
    SetGameState(&ai->game, new_state);

    if (ai->loglevel >= LOGLEVEL_INFO)
    {
        PrintTableInfo(&ai->game, ai->logfile);
    }
}

/*
 * Manually set the AI's hand cards
 * ai: the pokerAI to update
 * hand: the cards to give the AI
 * handsize: the number of cards in the AI's hand
 */
void SetHand(PokerAI *ai, char **hand, int handsize)
{
    ai->game.handsize = handsize;
    for (int i = 0; i < handsize; i++)
    {
        ai->game.hand[i] = StringToCard(hand[i]);
    }
}

/*
 * Manually set the AI's community cards
 * ai: the pokerAI to update
 * community: the cards to give the AI
 * communitysize: the number of community cards in play
 */
void SetCommunity(PokerAI *ai, char **community, int communitysize)
{
    ai->game.communitysize = communitysize;
    for (int i = 0; i < communitysize; i++)
    {
        ai->game.community[i] = StringToCard(community[i]);
    }
}

/*
 * Return whether or not it is the AI's turn to act
 * ai: the PokerAI to update
 * return: true if it is the AI's turn to act
 */
bool MyTurn(PokerAI *ai)
{
    return ai->game.your_turn;
}

/*
 * Use Monte Carlo simulation to determine the best action to take
 * ai: the AI that is predicting the best action
 * return: a string representation of the best action to take
 */
char *GetBestAction(PokerAI *ai)
{
    double winprob;
    double potodds;
    double expectedgain;
    ai->games_won = 0;
    ai->games_simulated = 0;

    //Set the pot odds
    if (ai->game.call_amount > 0)
    {
        potodds = (double) ai->game.call_amount / (ai->game.call_amount + ai->game.current_pot);
    }
    else
    {
        //There is nothing to be lost by calling
        potodds = 1.0 / ai->game.num_playing;
    }

    //Set the rate of return
    winprob = GetWinProbability(ai);
    expectedgain = winprob / potodds;

    if (ai->loglevel >= LOGLEVEL_INFO)
    {
        fprintf(ai->logfile, "Win probability: %.2lf%%\n", winprob * 100);
        fprintf(ai->logfile, "Rate of return:  %.2lf\n", expectedgain);
    }

    ai->action.winprob = winprob;
    ai->action.expectedgain = expectedgain;

    MakeDecision(ai);
    return ActionGetString(&ai->action);
}

/*
 * Use Monte Carlo simulation to determine the win probability given the AI's hand and community cards
 * ai: the AI that is predicting the win probability
 * return: the win probability as a double in the range [0, 1]
 */
double GetWinProbability(PokerAI *ai)
{
    double winprob;
    ai->games_won = 0;
    ai->games_simulated = 0;

    //Use preflop statistics if there aren't any community cards yet
    if (ai->game.communitysize == 0)
    {
        if (ai->loglevel >= LOGLEVEL_DEBUG)
        {
            fprintf(ai->logfile, "Performing simple preflop computation.\n");
        }

        winprob = PreflopWinProbability(ai->game.hand);
    }
    //Otherwise, start spawning Monte Carlo threads
    else
    {
        if (ai->loglevel >= LOGLEVEL_DEBUG)
        {
            fprintf(ai->logfile, "Performing Monte Carlo simulations.\n");
        }

        SpawnMonteCarloThreads(ai);
        winprob = ((double) ai->games_won) / ai->games_simulated;

        if (ai->loglevel >= LOGLEVEL_INFO)
        {
            //Human-readable output
            if (ai->games_simulated > 1000)
            {
                fprintf(ai->logfile, "Simulated %dk games.\n", ai->games_simulated / 1000);
            }
            else
            {
                fprintf(ai->logfile, "Simulated %d games.\n", ai->games_simulated);
            }
        }
    }

    return winprob;
}

/*
 * Print the AI's decision to the given FILE
 * ai: the AI that is making the decision
 * file: the FILE where output should be written
 */
void WriteAction(PokerAI *ai, FILE *file)
{
    switch(ai->action.type)
    {
    case ACTION_FOLD:
        fprintf(file, "ACTION:\tFOLDING\n");
        break;

    case ACTION_CALL:
        if (ai->action.bluff)
        {
            fprintf(file, "ACTION:\tCALLING (BLUFF)\n");
        }
        else
        {
            fprintf(file, "ACTION:\tCALLING\n");
        }
        break;

    case ACTION_BET:
        if (ai->action.bluff)
        {
            fprintf(file, "ACTION:\tBETTING %d (BLUFF)\n", ai->action.amount);
        }
        else
        {
            fprintf(file, "ACTION:\tBETTING %d\n", ai->action.amount);
        }
        break;

    default:
        fprintf(file, "!! No action set !!\n");
        break;
    }
}

/*
 * Calculate the preflop win probability based on hole cards
 * hand: the AI's hole cards
 * return: the win probability of the given hole cards
 */
static
double PreflopWinProbability(int *hand)
{
    /*
     * Quick guide:
     * val:     0  1  2  3  4  5  6  7  8  9  10 11 12
     * card:    2  3  4  5  6  7  8  9  T  J  Q  K  A
     * score([AH,AD]) = 30
     * score([AH,KD]) = 23
     * score([TD,8D]) = 18
     * score([AH,TD]) = 16
     * score([KH,TD]) = 16
     * score([JD,7D]) = 14
     * score([4D,5D]) = 9
     * score([2H,2D]) = 6
     * score([2H,7H]) = 5
     * score([2H,7D]) = 1
     */

    int score  = 0;
    int c1val  = (hand[0] - 1) / 4;
    int c2val  = (hand[1] - 1) / 4;
    int c1suit = (hand[0] - 1) % 4;
    int c2suit = (hand[1] - 1) % 4;
    int diff = abs(c1val - c2val);

    //Be warned: many magic numbers exist in this function
    //They have been chosen based on logic fine-tuning

    score = c1val + c2val;

    //Give bonuses to valued pairs and suited pairs
    if (c1val == c2val)
    {
        score += 6;
    }
    else if (c1suit == c2suit)
    {
        score += 4;
    }

    //Lower the score of cards with a large value difference
    if (diff > 4)
    {
        score -= 4;
    }
    else if (diff > 2)
    {
        score -= diff;
    }

    return score / 30.0;
}

/*
 * Spawn Monte Carlo threads to simulate poker games
 * ai: the AI which should spawn the threads
 */
static
void SpawnMonteCarloThreads(PokerAI *ai)
{
    if (ai->loglevel >= LOGLEVEL_DEBUG)
    {
        fprintf(ai->logfile, "Spawning Monte Carlo threads.\n");
    }

    //Spawn threads to perform Monte Carlo simulations
    for (int i = 0; i < ai->num_threads; i++)
    {
        pthread_create(&ai->threads[i], NULL, SimulateGames, ai);
    }

    //Wait until each thread has finished simulating games
    for (int i = 0; i < ai->num_threads; i++)
    {
        pthread_join(ai->threads[i], NULL);
    }

    if (ai->loglevel >= LOGLEVEL_DEBUG)
    {
        fprintf(ai->logfile, "All Monte Carlo threads finished.\n");
    }
}

/*
 * Simulate games for the given AI
 * _ai: a void pointer to a PokerAI pointer
 * return: NULL (pthread requirement)
 */
static
void *SimulateGames(void *_ai)
{
    PokerAI *ai = (PokerAI *)_ai;
    Timer timer;

    if (ai->loglevel >= LOGLEVEL_DEBUG)
    {
        fprintf(ai->logfile, "[Thread %u] starting\n", THREAD_ID);
    }

    int simulated = 0;
    int won = 0;

    StartTimer(&timer);
    //Only check the timer after every 5 simulations
    while (1)
    {
        if (simulated % 1000 == 0 && GetElapsedTime(&timer) > ai->timeout)
        {
            break;
        }

        won += SimulateSingleGame(ai);
        simulated++;
    }

    if (ai->loglevel >= LOGLEVEL_DEBUG)
    {
        fprintf(ai->logfile, "[Thread %u] done\t(simulated %d games)\n", THREAD_ID, simulated);
    }

    //Lock the AI mutex and update the totals
    pthread_mutex_lock(&ai->mutex);
    ai->games_won += won;
    ai->games_simulated += simulated;
    pthread_mutex_unlock(&ai->mutex);

    return NULL;
}

/*
 * Simulate a single poker game for the given AI
 * ai: the poker AI to simulate games for
 * return: AI_WIN on AI win or AI_LOSE on AI lose
 */
static
int SimulateSingleGame(PokerAI *ai)
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

    //Count ties as a win
    return (myscore >= bestopponent);
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
 * Set the AI's action given its expected gain
 * ai: the AI to set the action for
 * winprob: the AI's probability of winning the hand
 * expectedgain: the expected gain of the hand
 */
static
void MakeDecision(PokerAI *ai)
{
    double winprob = ai->action.winprob;
    double expectedgain = ai->action.expectedgain;

    int randnum = rand() % 100;
    int maxbet = (int)(ai->game.stack / 1.75) - (randnum / 2);
    maxbet -= ai->game.call_amount;

    //Don't bet too much on a bluff
    int bluffbet = randnum * maxbet / 100 / 2;

    if (expectedgain < 0.8 && winprob < 0.8)
    {
        if (randnum < 95)
        {
            ai->action.type = ACTION_FOLD;
        }
        else
        {
            ai->action.type = ACTION_BET;
            ai->action.bluff = true;
            ai->action.amount = bluffbet;
        }
    }
    else if ((expectedgain < 1.0 && winprob < 0.85) || winprob < 0.1)
    {
        if (randnum < 80)
        {
            ai->action.type = ACTION_FOLD;
        }
        else if (randnum < 5)
        {
            ai->action.type = ACTION_CALL;
            ai->action.bluff = true;
        }
        else
        {
            ai->action.type = ACTION_BET;
            ai->action.bluff = true;
            ai->action.amount = bluffbet;
        }
    }
    else if ((expectedgain < 1.3 && winprob < 0.9) || winprob < 0.5)
    {
        if (randnum < 60 || winprob < 0.5)
        {
            ai->action.type = ACTION_CALL;
            ai->action.bluff = false;
        }
        else
        {
            ai->action.type = ACTION_BET;
            ai->action.bluff = false;
            ai->action.amount = maxbet;
        }
    }
    else if (winprob < 0.95 || ai->game.communitysize < 4)
    {
        if (randnum < 30)
        {
            ai->action.type = ACTION_CALL;
            ai->action.bluff = false;
        }
        else
        {
            ai->action.type = ACTION_BET;
            ai->action.bluff = false;
            ai->action.amount = maxbet;
        }
    }
    else //huge chance of winning, okay to bet more than "maxbet"
    {
        maxbet = (ai->game.stack - ai->game.call_amount) * 9 / 10;
        ai->action.type = ACTION_BET;
        ai->action.bluff = false;
        ai->action.amount = maxbet;
    }

    //If our max bet is less than the call amount, just call instead
    if ((ai->action.type == ACTION_BET) && (maxbet < 0))
    {
        ai->action.type = ACTION_CALL;
    }

    //Don't fold if it's free to play
    if ((ai->action.type == ACTION_FOLD) && (ai->game.call_amount == 0))
    {
        ai->action.type = ACTION_CALL;
    }

    //Don't get locked in a raise loop
    if ((ai->action.type == ACTION_BET) && (ai->num_times_raised > NUM_RAISE_LIMIT))
    {
        ai->action.type = ACTION_CALL;
    }
}
