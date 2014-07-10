#include <stdio.h>

#include "tests.h"

#define TIMEOUT     1000

int main(int argc, char **argv)
{
    char *handranksfile = DEFAULT_HANDRANKS_FILE;

    if (argc == 2)
    {
        handranksfile = argv[1];
    }

    InitEvaluator(handranksfile);
    BeginConnectionSession();

    TestResult *result;
    int failed = 0;
    int numtests = 0;

    //Any argument will run the component testing suite
    //instead of the poker AI test
    if (argc == 2)
    {
        result = TestAction();
        failed += result->failed;
        numtests += result->numtests;
        DeleteResult(result);

        result = TestEvaluator();
        failed += result->failed;
        numtests += result->numtests;
        DeleteResult(result);

        result = TestGameState();
        failed += result->failed;
        numtests += result->numtests;
        DeleteResult(result);

        result = TestTimer();
        failed += result->failed;
        numtests += result->numtests;
        DeleteResult(result);

        result = TestURLConnection();
        failed += result->failed;
        numtests += result->numtests;
        DeleteResult(result);

        printf("\n\n");
        printf("=================\n");
        printf("||PASSED|FAILED||\n");
        printf("||%6d|%6d||\n", (numtests - failed), failed);
        printf("=================\n");
    }
    else
    {
        TestPokerAI(TIMEOUT);
    }

    EndConnectionSession();
    return 0;
}
