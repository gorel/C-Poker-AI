#include <stdio.h>
#include <unistd.h>

#include "evaluator.h"
#include "pokerai.h"
#include "urlconnection.h"

#define NUM_THREADS 4
#define TIMEOUT     1000
#define GET_URL     "http://example.com/"
#define POST_URL    "http://example.com/post/"
#define MAX_TRIES   5
#define BUF_SIZE    1024

#define PRINTERR(...) fprintf(stderr, __VA_ARGS__)

/*
 * Set up everything necessary for the client
 */
static
void PokerClientSetup(void);

/*
 * Shut down all resources for the client
 */
static
void PokerClientShutdown(void);

int main(int argc, char **argv)
{
    PokerAI *AI = NULL;
    cJSON *response = NULL;
    char *action = NULL;
    char postURL[BUF_SIZE] = {0};

    //Set up the poker client
    PokerClientSetup();
    AI = CreatePokerAI(NUM_THREADS, TIMEOUT);

    while (1)
    {
        //Get the game state
        response = httpGetJSON(GET_URL);
        if (!response)
        {
            PRINTERR("Could not load game state!\n");
            sleep(1);
            continue;
        }

        UpdateGameState(AI, response);
        cJSON_Delete(response);

        //If it's the AI's turn, make a decision
        if (MyTurn(AI))
        {
            int attempts = 0;

            //Run Monte Carlo simulations to determine the best action
            action = GetBestAction(AI);
            sprintf(postURL, "%s%s", POST_URL, action);
            WriteAction(AI, stdout);

            //Post the action to the server
            while (attempts < MAX_TRIES && !response)
            {
                response = NULL;
                response = httpPostJSON(postURL, NULL);
                attempts++;

                if (!response)
                {
                    PRINTERR("Could not POST response (attempt %d)\n", attempts);
                }
                cJSON_Delete(response);
            }

            if (attempts == MAX_TRIES)
            {
                PRINTERR("Was not able to POST!\n");
            }
        }

        //Wait one second before updating game state again
        sleep(1);
    }

    //Clean up resources
    DestroyPokerAI(AI);
    PokerClientShutdown();
    return 0;
}

/*
 * Set up everything necessary for the client
 */
static
void PokerClientSetup(void)
{
    printf("Initializing poker tables...\t");
    InitEvaluator();
    printf("Tables initialized\n");

    printf("Starting curl session...\t");
    BeginConnectionSession();
    printf("Session started\n");

    printf("\nPoker client running\n\n");
}

/*
 * Shut down all resources for the client
 */
static
void PokerClientShutdown(void)
{
    printf("Ending curl session...\t");
    EndConnectionSession();
    printf("Session ended\n");
}
