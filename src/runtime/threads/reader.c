#include "reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime.h"

void *reader_func(void *vargp) {
  (void)vargp;
  while (1) {
    printf("Hello reader\n");
    sleep(1);
    int ret = pt_queue_enqueue(pt_queue_reader_analyzer, "Hello reader - enqueue");
    if (ret != 0) {
      printf("Error enqueueing\n");
    }
  }

  return NULL;
}