#ifndef __ACTION_H__
#define __ACTION_H__

#include <stdio.h>

//Enum representing the possible action types
typedef enum action_type
{
    FOLD,
    CALL,
    BET,
    ACTION_UNSET
} ActionType;

//struct representing an action to take
typedef struct action
{
    ActionType type;
    int amount;

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
