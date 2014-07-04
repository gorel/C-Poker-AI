#include <stdio.h>

#include "tests.h"

int main(int argc, char **argv)
{
    InitEvaluator();
    BeginConnectionSession();

    TestResult *result;
    int failed = 0;
    int numtests = 0;

    int timeout;
    if (argc == 2)
    {
        timeout = atoi(argv[1]);
        TestPokerAI(timeout);
    }
    else
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

    EndConnectionSession();
    return 0;
}
