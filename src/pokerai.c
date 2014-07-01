#include "pokerai.h"

#define AI_WIN  1
#define AI_LOSE 0

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

    return ai;
}

/*
 * Destroy the PokerAI and all associated memory
 * ai: the PokerAI to destroy
 */
void DestroyPokerAI(PokerAI *ai)
{
    if (!ai) return;

    pthread_mutex_destroy(&ai->mutex);
    free(ai->threads);
    free(ai);
}

/*
 * Set debug logging to the given level
 * and output to the given file descriptor
 * ai: the AI to set logging for
 * level: the logging level of output
 * fd: the file descriptor where output is sent
 */
void SetLogging(PokerAI *ai, LOGLEVEL level, int fd)
{
    ai->loglevel = level;
    ai->loggingfd = fd;
}

/*
 * Update the given PokerAI's game state
 * ai: the PokerAI to update
 * new_state: JSON representation of the new game state
 */
void UpdateGameState(PokerAI *ai, cJSON *new_state)
{
    SetGameState(&ai->game, new_state);
    ai->action.type = ACTION_UNSET;
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
    ai->games_won = 0;
    ai->games_simulated = 0;

    SpawnMonteCarloThreads(ai);
    //TODO: Decide on an action
    return ActionGetString(&ai->action);
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
    case FOLD:
        fprintf(file, "ACTION:\tFOLDING\n");
        break;

    case CALL:
        fprintf(file, "ACTION:\tCALLING\n");
        break;

    case BET:
        fprintf(file, "ACTION:\tBETTING %d\n", ai->action.amount);
        break;

    default:
        fprintf(file, "No action set\n");
        break;
    }
}

/*
 * Spawn Monte Carlo threads to simulate poker games
 * ai: the AI which should spawn the threads
 */
static
void SpawnMonteCarloThreads(PokerAI *ai)
{
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

    int simulated = 0;
    int won = 0;

    StartTimer(&timer);
    //Only check the timer after every 5 simulations
    for (simulated = 0;
        (simulated % 5) && (GetElapsedTime(&timer) < ai->timeout);
         simulated++)
    {
        won += SimulateSingleGame(ai);
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
   //TODO: Simulate a single Poker Game
   return AI_LOSE;
}
