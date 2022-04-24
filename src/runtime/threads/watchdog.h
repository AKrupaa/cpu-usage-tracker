#ifndef WATCHDOG_THREADS_H
#define WATCHDOG_THREADS_H
#include <pthread.h>
#include <stdbool.h>

void *watchdog_func(void *vargp);
void check_threads(void);

#endif  // WATCHDOG_THREADS_H
