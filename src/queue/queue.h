#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <string.h>

// A queue FIFO structure, first-in, first-out
typedef struct queue_t {
  char *data;
  size_t size;
  size_t capacity;
  size_t head;
  size_t tail;
} queue_t;

// Pop the first element from the queue
char *queue_dequeue(queue_t *queue, size_t data_size);

// enqueue
// Add an element to the tail of the queue
// Return 0 if success, -1 if failed
int queue_enqueue(queue_t *queue, char *data, size_t size);

queue_t *queue_create(size_t capacity);

int queue_get_size(queue_t *queue);
int queue_get_capacity(queue_t *queue);

int queue_destroy(queue_t *queue);

#endif  // QUEUE_H
