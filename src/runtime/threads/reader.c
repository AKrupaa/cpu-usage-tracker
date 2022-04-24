#include "reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu_info.h"
#include "runtime.h"

void *reader_func(void *vargp) {
  (void)vargp;

  int cpus = get_cpu_count();
  int buffer_size = sizeof(char) * (PROC_LINE_LENGTH + 1) * cpus;
  char *buffer = malloc(buffer_size);

  while (1) {
    pt_set_alive(pt_mutex_reader_alive);
    char *raw_cpu_info = get_CPU_raw_data(buffer, cpus);

    if (raw_cpu_info[0] == '\0') {
      // TODO: logger
      continue;
    }

    // TODO: logger

    int ret =
        pt_queue_enqueue(pt_queue_reader_analyzer, raw_cpu_info, buffer_size);

    if (ret != 0) {
      // TODO: logger
      printf("Error enqueueing\n");
    }

    sleep(1);
  }

  return NULL;
}