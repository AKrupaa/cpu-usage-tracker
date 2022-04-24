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

  while (1) {
    pt_set_alive(pt_mutex_analyzer_alive);

    char *old = pt_queue_dequeue(pt_queue_reader_analyzer,
                                 (PROC_LINE_LENGTH + 1) * cpus);
    char *new = pt_queue_dequeue(pt_queue_reader_analyzer,
                                 (PROC_LINE_LENGTH + 1) * cpus);

    if (get_percentage_usage_of_CPU(old, new, buffer) != 0) {
      printf("Error calculating\n");
      continue;
    }

    int ret = pt_queue_enqueue(pt_queue_analyzer_printer, buffer, buffer_size);

    if (ret != 0) {
      printf("Error enqueueing\n");
    }
  }

  return NULL;
}