#include "evaluator.h"

bool POKERLIB_INITIALIZED = false;

/*
 * Initialize the 2+2 evaluator by loading the lookup table
 * into the HR array.
 */
void InitEvaluator(void)
{
    //Make sure not to load the array twice
    if (POKERLIB_INITIALIZED) return;

    memset(HR, 0, sizeof(HR));
    FILE *in = fopen("HANDRANKS.DAT", "rb");
    fread(HR, sizeof(HR), 1, in);
    fclose(in);

    POKERLIB_INITIALIZED = true;
}

/*
 * Evaluate a hand of 5, 6, or 7 cards
 * cards: an array of 5, 6, or 7 cards
 * num_cards: the number of cards given
 * return: an int representing the relative rank of the hand
 * Higher number == stronger hand
 */
int GetHandValue(int *cards, int num_cards)
{
    int p = HR[53 + cards[0]];
    for (int i = 1; i < num_cards; i++)
    {
        p = HR[p + cards[i]];
    }

    return p;
}
