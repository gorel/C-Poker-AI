#include "timer.h"

#define MICROSECONDS(timeval) \
    (timeval.tv_usec + 1000000 * timeval.tv_sec)
#define MICRO_TO_MILLI(X) \
    (X / 1000)

/*
 * Start the timer
 * timer: the timer to start
 */
void StartTimer(Timer *timer)
{
    gettimeofday(&timer->begin, NULL);
    timer->state = TIMER_RUNNING;
}

/*
 * Stop the timer
 * timer: the timer to stop
 */
void StopTimer(Timer *timer)
{
    unsigned long long diff;
    struct timeval end;
    gettimeofday(&end, NULL);

    diff = MICROSECONDS(end) - MICROSECONDS(timer->begin);
    timer->elapsed = MICRO_TO_MILLI(diff);
    timer->state = TIMER_STOPPED;
}

/*
 * Get the elapsed time since the timer was started
 * (or the elapsed time between timer start and timer stop)
 * timer: the timer to check the elapsed time for
 * return: the elapsed time in milliseconds
 */
unsigned long long GetElapsedTime(Timer *timer)
{
    if (timer->state == TIMER_RUNNING)
    {
        unsigned long long diff;
        struct timeval now;
        gettimeofday(&now, NULL);

        diff = MICROSECONDS(now) - MICROSECONDS(timer->begin);
        timer->elapsed = MICRO_TO_MILLI(diff);
    }

    return timer->elapsed;
}
