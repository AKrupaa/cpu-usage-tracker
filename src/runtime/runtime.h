#ifndef RUNTIME_H
#define RUNTIME_H

#include <pthread.h>
#include <stdbool.h>

#include "configure.h"
#include "queue.h"

typedef struct _pt_thread_def_ {
  char *name;
  const pthread_attr_t *attr;
  void *(*func)(void *);
  void *arg;
  // pthread_mutex_t mutex_alive;
  // bool alive;
} pt_thread_def_t;

typedef struct _pt_queue_def_ {
  pthread_mutex_t mutex;
  pthread_cond_t less;
  pthread_cond_t more;
  queue_t *queue;
  char *name;
  size_t size;
  bool is_block;
  bool two_way;
} pt_queue_def_t;

void pt_thread_init(void);
void pt_thread_join(void);
void pt_queue_init(void);
void pt_runtime_init(void);
char *pt_queue_dequeue(pt_queue_t queue, size_t data_size);
int pt_queue_enqueue(pt_queue_t queue, char *data, size_t size);
bool pt_is_alive(pt_mutex_t mutex);
void pt_set_alive(pt_mutex_t mutex, bool status);
void pt_set_dead(pt_mutex_t mutex);

extern pt_thread_def_t const pt_thread_def[pt_thread_N];
extern pt_queue_def_t const pt_queue_def[pt_queue_N];

#endif /* RUNTIME_H */
