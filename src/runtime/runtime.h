#ifndef RUNTIME_H
#define RUNTIME_H

#include <pthread.h>
#include <stdbool.h>
#include "configure.h"
#include "queue.h"

typedef struct _pt_thread_def_ {
  char* name;
  const pthread_attr_t *attr;
  void *(*func)(void *);
  void *arg;
} pt_thread_def_t;

typedef struct _pt_queue_def_ {
  char* name;
  int size;
  bool is_block;
  bool two_way;
  pthread_mutex_t mutex;
  pthread_cond_t less;
  pthread_cond_t more;
  queue_t *queue;
} pt_queue_def_t;

void pt_thread_init(void);
void pt_thread_join(void);
void pt_queue_init(void);
char *pt_queue_dequeue(pt_queue_t queue, int data_size);
int pt_queue_enqueue(pt_queue_t queue, char *data, int size);

extern pt_thread_def_t const pt_thread_def[pt_thread_N];
extern pt_queue_def_t const pt_queue_def[pt_queue_N];

#endif /* RUNTIME_H */