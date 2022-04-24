#include "watchdog.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime.h"
void *watchdog_func(void *vargp) {
  (void)vargp;
  while (pt_is_alive(pt_mutex_watchdog_alive) || !it_done()) {
    pt_set_alive(pt_mutex_watchdog_alive, true);
    sleep(2);
    check_threads();
  }

  return NULL;
}

void check_threads(void) {
  for (pt_mutex_t alive = pt_mutex_threads_alive_0;
       alive < pt_mutex_threads_alive_N; alive++) {
    if (!pt_is_alive(alive)) {
      fprintf(stderr, "Thread %d is dead!\n", alive - pt_mutex_queue_N);
      runtime_deinit();
      printf("Exiting...\n");
      exit(1);
    }
    pt_set_alive(alive, false);
  }
}
