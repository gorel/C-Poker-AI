#include <stdio.h>

#include "evaluator.h"
#include "pokerai.h"

#define DEFAULT_NUM_PLAYING 3
#define TIMEOUT             1000

int main(int argc, char **argv)
{
    char *handranksfile = DEFAULT_HANDRANKS_FILE;
    char *hand[NUM_HAND];
    char *community[NUM_COMMUNITY];
    char *last_arg;
    int num_community = argc - 3;
    int num_playing = DEFAULT_NUM_PLAYING;
    double winprob;
    PokerAI *AI;

    InitEvaluator(handranksfile);

    if (argc < 3)
    {
        fprintf(stderr, "Usage: ./winprob hand1 hand2 [comm1, .. , comm5] [-nx]\n");
        fprintf(stderr, "\tWhere x is the number of opponents (3 by default)\n");
        exit(1);
    }

    for (int i = 0; i < NUM_HAND; i++)
    {
        hand[i] = argv[i + 1];
    }

    for (int i = 0; i < num_community - 1; i++)
    {
        community[i] = argv[NUM_HAND + i + 1];
    }

    //Check if the last card is a community card
    //or a specifier of the number of players
    last_arg = argv[NUM_HAND + num_community];
    if (!strncmp(last_arg, "-n", strlen("-n")))
    {
        num_playing = atoi(last_arg + strlen("-n"));
        num_community--;
    }
    else
    {
        community[num_community - 1] = last_arg;
    }

    AI = CreatePokerAI(TIMEOUT);
    SetHand(AI, hand, NUM_HAND);
    SetCommunity(AI, community, num_community);
    UpdateGameDeck(&AI->game);
    AI->game.num_playing = num_playing;
    winprob = GetWinProbability(AI);
    printf("Win probability: %.2lf%%\n", winprob * 100);

    //Clean up resources
    DestroyPokerAI(AI);
    return 0;
}
