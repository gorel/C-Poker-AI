#include "tests.h"

TestResult *CreateResult(int failed, int numtests)
{
    TestResult *result = malloc(sizeof(TestResult));
    result->failed = failed;
    result->numtests = numtests;

    return result;
}

void DeleteResult(TestResult *result)
{
    if (result)
    {
        free(result);
    }
}
