#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "runtime/runtime.h"

int main(int argc, char const* argv[]) {
  /* unused parameter */
  (void)argc;
  (void)argv;

  printf("Hello World!\n");

  pt_queue_init();
  pt_thread_init();

  pt_thread_join();

  printf("Goodbye World!\n");

  return 0;
}
