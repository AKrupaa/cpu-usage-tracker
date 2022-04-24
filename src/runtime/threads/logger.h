#ifndef LOGGER_THREADS_H
#define LOGGER_THREADS_H
#include <pthread.h>
#include <stdbool.h>

#define LOGGER_FILE_NAME "logger.log"

void *logger_func(void *vargp);

#endif  // LOGGER_THREADS_H