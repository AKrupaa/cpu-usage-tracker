#include "runtime.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "configure.h"
#include "cpu_info.h"

pthread_t pt_threads[pt_thread_N];
pt_queue_def_t pt_queues[pt_queue_N];
pthread_cond_t pt_conds[2 * pt_queue_N] = {PTHREAD_COND_INITIALIZER};
pthread_mutex_t pt_mutexs[pt_queue_N] = {PTHREAD_MUTEX_INITIALIZER};

static int queue_init(pt_queue_def_t *queue, int size);

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

void pt_queue_init(void) {
  for (pt_queue_t queue = pt_queue_0; queue < pt_queue_N; queue++) {
    pt_queue_def_t const *def = pt_queue_def + queue;
    pt_queue_def_t *q = pt_queues + queue;
    *q = *def;

    if (queue == pt_queue_reader_analyzer) {
      int cpus = get_cpu_count();
      q->size = (PROC_LINE_LENGTH + 1) * cpus * 4;
    }

    if (queue_init(q, q->size) != 0)
      while (1)
        ;

    if (q->is_block == true) {
      pthread_cond_t *less = pt_conds + 2 * queue;
      pthread_cond_t *more = pt_conds + 2 * queue + 1;
      q->less = *less;
      q->more = *more;

      pthread_mutex_t const *m = pt_mutexs + queue;
      q->mutex = *m;
    }
  }
}

char *pt_queue_dequeue(pt_queue_t queue, int data_size) {
  pt_queue_def_t *q = pt_queues + queue;
  pthread_mutex_lock(&q->mutex);
  while (queue_get_size(q->queue) == 0) {
    pthread_cond_wait(&q->more, &q->mutex);
  }
  char *data = queue_dequeue(q->queue, data_size);

  pthread_cond_signal(&q->less);
  pthread_mutex_unlock(&q->mutex);

  return data;
}

int pt_queue_enqueue(pt_queue_t queue, char *data, int size) {
  pt_queue_def_t *q = pt_queues + queue;
  pthread_mutex_lock(&q->mutex);

  while (queue_get_size(q->queue) >= queue_get_capacity(q->queue)) {
    pthread_cond_wait(&q->less, &q->mutex);
  }

  int ret = queue_enqueue(q->queue, data, size);
  pthread_cond_signal(&q->more);

  pthread_mutex_unlock(&q->mutex);

  return ret;
}

static int queue_init(pt_queue_def_t *queue, int size) {
  queue->queue = queue_create(size);
  if (queue->queue == NULL) {
    return -1;
  }
  return 0;
}