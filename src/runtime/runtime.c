#include "runtime.h"

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "configure.h"
#include "cpu_info.h"

// volatile static sig_atomic_t done = 0;

void term(int signum);

static pthread_t pt_threads[pt_thread_N];
static pt_queue_def_t pt_queues[pt_queue_N];
static pthread_cond_t pt_conds[2 * pt_queue_N] = {PTHREAD_COND_INITIALIZER};
static pthread_mutex_t pt_mutexs[pt_mutex_N] = {PTHREAD_MUTEX_INITIALIZER};
static bool pt_thread_alive[pt_thread_N] = {true};

static int queue_init(pt_queue_def_t *queue, size_t size);
int queue_deinit(pt_queue_def_t *queue);

void pt_runtime_init(void) {
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = term;
  sigaction(SIGTERM, &action, NULL);

  pt_queue_init();
  pt_thread_init();
}

void pt_thread_init(void) {
  for (pt_thread_t thread = pt_thread_0; thread < pt_thread_N; thread++) {
    pt_thread_def_t const *def = pt_thread_def + thread;
    pthread_t *pt = pt_threads + thread;

    pt_thread_alive[thread] = true;

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

bool pt_is_alive(pt_mutex_t mutex) {
  // pthread_mutex_lock(&pt_thread_def[thread].mutex_alive);
  // int alive = pt_thread_def[thread].mutex_alive.__data.__count;
  // pthread_mutex_unlock(&pt_thread_def[thread].mutex_alive);
  pthread_mutex_t *def = pt_mutexs + mutex;
  pthread_mutex_lock(def);
  bool alive = pt_thread_alive[mutex - pt_mutex_queue_N];
  pthread_mutex_unlock(def);

  return alive;
}

void pt_set_alive(pt_mutex_t mutex, bool status) {
  pthread_mutex_t *def = pt_mutexs + mutex;
  pthread_mutex_lock(def);

  pt_thread_alive[mutex - pt_mutex_queue_N] = status;

  pthread_mutex_unlock(def);
}

// void pt_mutex_init(void) {
//   for (pt_mutex_t mut = pt_mutex_0; mut < pt_mutex_N; mut++) {
//     pthread_mutex_t *mutex = pt_mutexs + mut;
//     if (pthread_mutex_init(mutex, NULL) != 0)
//       while (1)
//         ;
//   }
// }

void pt_queue_init(void) {
  for (pt_queue_t queue = pt_queue_0; queue < pt_queue_N; queue++) {
    pt_queue_def_t const *def = pt_queue_def + queue;
    pt_queue_def_t *q = pt_queues + queue;
    *q = *def;

    if (queue == pt_queue_reader_analyzer) {
      unsigned int cpus = get_cpu_count();
      q->size = (size_t)(PROC_LINE_LENGTH + 1) * cpus * 4;
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

char *pt_queue_dequeue(pt_queue_t queue, size_t data_size) {
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

int pt_queue_enqueue(pt_queue_t queue, char *data, size_t size) {
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

static int queue_init(pt_queue_def_t *queue, size_t size) {
  queue->queue = queue_create(size);
  if (queue->queue == NULL) {
    return -1;
  }
  return 0;
}

int queue_deinit(pt_queue_def_t *queue) {
  queue_destroy(queue->queue);
  return 0;
}

void term(int signum) {
  if (signum != SIGTERM) return;

  for (pt_mutex_t alive = pt_mutex_threads_alive_0;
       alive < pt_mutex_threads_alive_N; alive++) {
    pt_set_alive(alive, false);
    while (pt_is_alive(alive))
      ;
  }

  for (pt_queue_t queue = pt_queue_0; queue < pt_queue_N; queue++) {
    pt_queue_def_t *q = pt_queues + queue;
    queue_deinit(q);
  }

  printf("Exiting...\n");
  exit(0);
}
