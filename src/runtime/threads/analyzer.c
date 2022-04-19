#include "analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime.h"

void *analyzer_func(void *vargp) {
  (void)vargp;
  while (1) {
    printf("Hello analyzer\n");
    sleep(10);
    char* text = pt_queue_dequeue(pt_queue_reader_analyzer);
    if (text != NULL) {
      printf("Analyzer: %s\n", text);
    }
  }

  return NULL;
}