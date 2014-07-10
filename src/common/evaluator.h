#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_HANDRANKS_FILE  "HANDRANKS.DAT"
#define COLOR_ERROR "\033[1;31m"
#define COLOR_DEFAULT "\033[0m"

//Massive lookup table
int HR[32487834];

//We only want to initialize the lookup table once
extern bool POKERLIB_INITIALIZED;

/*
 * Initialize the 2+2 evaluator by loading the lookup table
 * into the HR array.
 * handranksfile: the hand ranks look up table data
 */
void InitEvaluator(char *handranksfile);

/*
 * Evaluate a hand of 5, 6, or 7 cards
 * cards: an array of 5, 6, or 7 cards
 * num_cards: the number of cards given
 * return: an int representing the relative rank of the hand
 * Higher number == stronger hand
 */
int GetHandValue(int *cards, int num_cards);

#endif
