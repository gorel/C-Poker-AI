#include "tests.h"

//Allow an error of 10 milliseconds
#define TIMER_EPSILON   0.01

//Number of milliseconds in a second
#define SECOND          1000

TestResult *TestTimer(void)
{
    int numtests = 0;
    int failed = 0;
    Timer timer;

    StartTimer(&timer);
    sleep(1);
    StopTimer(&timer);
    if (abs(GetElapsedTime(&timer) - SECOND) > TIMER_EPSILON)
    {
        fprintf(stderr, "Failed test for stopped timer\n");
        failed++;
    }
    numtests++;

    StartTimer(&timer);
    sleep(1);
    if (abs(GetElapsedTime(&timer) - SECOND) > TIMER_EPSILON)
    {
        fprintf(stderr, "Failed test for running timer\n");
        failed++;
    }
    numtests++;

    fprintf(stderr, "[TIMER]\t\t\tpassed %d/%d\n", (numtests - failed), numtests);
    return CreateResult(failed, numtests);
}
