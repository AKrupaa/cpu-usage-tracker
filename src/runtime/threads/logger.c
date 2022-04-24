#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime.h"

void *logger_func(void *vargp) {
  (void)vargp;

  while (1) {
    pt_set_alive(pt_mutex_logger_alive);
    sleep(10);
  }

  return NULL;
}