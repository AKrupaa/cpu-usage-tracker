#include "analyzer.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cpu_info.h"
#include "runtime.h"

void *analyzer_func(void *vargp) {
  (void)vargp;

  int cpus = get_cpu_count();
  int buffer_size = (sizeof(char) * sizeof(double)) * cpus;
  char *buffer = malloc(buffer_size);
  char *msg = malloc(sizeof(char) * 30);

  while (pt_is_alive(pt_mutex_analyzer_alive)) {
    pt_set_alive(pt_mutex_analyzer_alive, true);

    char *old = pt_queue_dequeue(pt_queue_reader_analyzer,
                                 (PROC_LINE_LENGTH + 1) * cpus);
    char *new = pt_queue_dequeue(pt_queue_reader_analyzer,
                                 (PROC_LINE_LENGTH + 1) * cpus);

    msg = "Analisys in progress...";
    pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);

    if (get_percentage_usage_of_CPU(old, new, buffer) != 0) {
      msg = "Error: Calulating CPU usage failed.";
      pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);
      continue;
    }

    int ret = pt_queue_enqueue(pt_queue_analyzer_printer, buffer, buffer_size);
    if (ret != 0) {
      msg = "Error: Enqueueing CPU usage failed.";
      pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);
    }

    msg = "Analisys done.";
    pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);
  }


  free(buffer);
  free(msg);
  return NULL;
}