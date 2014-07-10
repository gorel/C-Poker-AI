#include "tests.h"

#define FOLD_RESULT "action_name=fold"
#define CALL_RESULT "action_name=call"
#define BET_AMOUNT  500
#define BET_RESULT  "action_name=bet&amount=500"

TestResult *TestAction(void)
{
    Action action;
    char *result;
    int numtests = 0;
    int failed = 0;

    ActionSetFold(&action);
    result = ActionGetString(&action);
    if (strcmp(result, FOLD_RESULT))
    {
        fprintf(stderr, "[ACTION] Failed FOLD test\n");
        failed++;
    }
    numtests++;

    ActionSetCall(&action);
    result = ActionGetString(&action);
    if (strcmp(result, CALL_RESULT))
    {
        fprintf(stderr, "[ACTION] Failed CALL test\n");
        failed++;
    }
    numtests++;

    ActionSetBet(&action, BET_AMOUNT);
    result = ActionGetString(&action);
    if (strcmp(result, BET_RESULT))
    {
        fprintf(stderr, "[ACTION] Failed BET test\n");
        failed++;
    }
    numtests++;

    fprintf(stderr, "[ACTION]\t\tpassed %d/%d\n", (numtests - failed), numtests);
    return CreateResult(failed, numtests);
}
