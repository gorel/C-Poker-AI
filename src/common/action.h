#ifndef __ACTION_H__
#define __ACTION_H__

#include <stdbool.h>
#include <stdio.h>

//Enum representing the possible action types
typedef enum action_type
{
    ACTION_FOLD,
    ACTION_CALL,
    ACTION_BET,
    ACTION_UNSET
} ActionType;

//struct representing an action to take
//and information about why the action was chosen
typedef struct action
{
    ActionType type;
    bool bluff;
    int amount;

    double winprob;
    double expectedgain;

    char string[256];
} Action;

/*
 * Set the action to fold
 * action: the Action to set
 */
void ActionSetFold(Action *action);

/*
 * Set the action to call
 * action: the Action to set
 */
void ActionSetCall(Action *action);

/*
 * Set the action to bet the given amount
 * action: the Action to set
 * amount: the size of the bet
 */
void ActionSetBet(Action *action, int amount);

/*
 * Get the string representation of the action
 * action: the Action to access
 */
char *ActionGetString(Action *action);

#endif
