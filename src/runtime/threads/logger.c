#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "runtime.h"

void *logger_func(void *vargp) {
  (void)vargp;

  while (pt_is_alive(pt_mutex_logger_alive)) {
    pt_set_alive(pt_mutex_logger_alive, true);

    FILE *logger_file = fopen(LOGGER_FILE_NAME, "a");

    char *buffer = pt_queue_dequeue(pt_queue_logger, 0);

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    fprintf(logger_file, "%s %s\n", asctime(timeinfo), buffer);

    fclose(logger_file);
  }

  return NULL;
}
