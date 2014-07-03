#include <stdio.h>

#include "tests.h"

int main(int argc, char **argv)
{
    InitEvaluator();

    int timeout = 1000;
    if (argc == 2)
    {
        timeout = atoi(argv[1]);
    }

    /*
    TestAction();
    TestEvaluator();
    TestGameState();
    TestTimer();
    TestURLConnection();
    */
    TestPokerAI(timeout);

    return 0;
}
