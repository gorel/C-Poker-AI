#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdlib.h>
#include <sys/time.h>

typedef enum timerstate
{
    TIMER_RUNNING,
    TIMER_STOPPED
} TimerState;

typedef struct timer
{
    TimerState state;
    struct timeval begin;
    unsigned long long elapsed;
} Timer;

/*
 * Start the timer
 * timer: the timer to start
 */
void StartTimer(Timer *timer);

/*
 * Stop the timer
 * timer: the timer to stop
 */
void StopTimer(Timer *timer);

/*
 * Get the elapsed time since the timer was started
 * (or the elapsed time between timer start and timer stop)
 * timer: the timer to check the elapsed time for
 * return: the elapsed time in milliseconds
 */
unsigned long long GetElapsedTime(Timer *timer);

#endif
