#include "runtime.h"

#include "configure.h"

pthread_t pt_threads[pt_thread_N];

void pt_thread_init(void) {
  for (pt_thread_t thread = pt_thread_0; thread < pt_thread_N; thread++) {
    pt_thread_def_t const *def = pt_thread_def + thread;
    pthread_t *pt = pt_threads + thread;

    if (pthread_create(pt, def->attr, def->func, def->arg) != 0)
      while (1)
        ;
  }
}

void pt_thread_join(void) {
  for (pt_thread_t thread = pt_thread_0; thread < pt_thread_N; thread++) {
    pthread_t *pt = pt_threads + thread;
    if (pthread_join(*pt, NULL) != 0)
      while (1)
        ;
  }
}