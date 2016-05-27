#ifndef WAIT_EVENT_H
#define WAIT_EVENT_H

#include <time.h>

/*
A WaitEvent is a structure that stores a time and a function.
The time is the time in which the function should be executed.
*/

typedef struct {
  time_t handleTime;
  time_t (*HandleEvent)();
} WaitEvent;

typedef struct {
  WaitEvent *sortedWaitEvents;
} WaitEventManager;


#endif
