#ifndef RUNTIME_H
#define RUNTIME_H

#include <pthread.h>
#include "configure.h"

typedef struct _pt_thread_def_ {
  char* name;
  const pthread_attr_t *attr;
  void *(*func)(void *);
  void *arg;
} pt_thread_def_t;

void pt_thread_init(void);
void pt_thread_join(void);

extern const pt_thread_def_t pt_thread_def[pt_thread_N];

#endif /* RUNTIME_H */