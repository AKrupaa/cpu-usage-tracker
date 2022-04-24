#include "reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu_info.h"
#include "runtime.h"

void *reader_func(void *vargp) {
  (void)vargp;

  unsigned int cpus = get_cpu_count();
  size_t buffer_size = (size_t)(sizeof(char) * (PROC_LINE_LENGTH + 1) * cpus);
  char *buffer = malloc(buffer_size);
  char *msg = malloc(sizeof(char) * 30);

  while (pt_is_alive(pt_mutex_reader_alive)) {
    pt_set_alive(pt_mutex_reader_alive, true);

    msg = "Reading in progress...";
    pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);

    char *raw_cpu_info = get_CPU_raw_data(buffer, cpus);

    if (raw_cpu_info[0] == '\0') {
      msg = "Error: Reading CPU data failed.";
      pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);
      continue;
    }

    msg = "Reading done.";
    pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);

    int ret =
        pt_queue_enqueue(pt_queue_reader_analyzer, raw_cpu_info, buffer_size);

    if (ret != 0) {
      msg = "Error: Enqueueing CPU data failed.";
      pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);
    }

    sleep(1);
  }

  free(msg);
  free(buffer);

  return NULL;
}
