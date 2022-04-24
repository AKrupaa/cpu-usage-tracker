#ifndef LOGGER_THREADS_H
#define LOGGER_THREADS_H
#include <pthread.h>
#include <stdbool.h>

void *logger_func(void *vargp);

#endif  // LOGGER_THREADS_H