#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pop the first element from the queue
char *queue_dequeue(queue_t *queue, size_t data_size) {
  if (queue == NULL) {
    return NULL;
  }
  if (queue->size == 0) {
    return NULL;
  }
  char *data = queue->data + queue->head;

  if (data_size == 0) {
    data_size = strlen(data) + 1;
  }

  queue->head = (queue->head + data_size) % queue->capacity;
  queue->size -= data_size;
  return data;
}

// enqueue
// Add an element to the tail of the queue
// Return 0 if success, -1 if failed
int queue_enqueue(queue_t *queue, char *data, size_t data_size) {
  if (queue == NULL || data == NULL) {
    return -1;
  }

  if (queue->size + data_size > queue->capacity) {
    return -1;
  }

  char *p = queue->data + queue->tail;
  // strcpy(p, data);
  memcpy(p, data, data_size);
  queue->tail = (queue->tail + data_size) % queue->capacity;
  queue->size += data_size;
  return 0;
}

// create a queue
queue_t *queue_create(size_t capacity) {
  queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
  if (queue == NULL) {
    return NULL;
  }
  queue->size = 0;
  queue->capacity = capacity;
  queue->head = 0;
  queue->tail = 0;
  queue->data = (char *)malloc(sizeof(char *) * capacity);
  if (queue->data == NULL) {
    free(queue);
    return NULL;
  }
  return queue;
}

int queue_get_size(queue_t *queue) {
  if (queue == NULL) {
    return -1;
  }
  return (int)queue->size;
}

int queue_get_capacity(queue_t *queue) {
  if (queue == NULL) {
    return -1;
  }
  return (int)queue->capacity;
}

int queue_destroy(queue_t *queue) {
  if (queue == NULL) {
    return -1;
  }
  free(queue->data);
  free(queue);
  return 0;
}
