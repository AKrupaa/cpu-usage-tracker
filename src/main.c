#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "runtime/runtime.h"

int main(int argc, char const* argv[]) {
  /* unused parameter */
  (void)argc;
  (void)argv;

  printf("Hello World!\n");

  // pthread_t Reader;
  // pthread_t Analzyer;
  // pthread_t Printer;
  // pthread_t Watchdog;
  // pthread_t Logger;

  // pthread_create(&Reader, NULL, Reader_main, NULL);
  // pthread_mutex_t Reader_mutex = PTHREAD_MUTEX_INITIALIZER;
  // // create mutex
  // pthread_mutex_t Analzyer_mutex = PTHREAD_MUTEX_INITIALIZER;

  // // get mutex
  // pthread_mutex_lock(&Analzyer_mutex);

  // // free mutex
  // pthread_mutex_unlock(&Analzyer_mutex);

  pt_thread_init();
  pt_thread_join();

  printf("Goodbye World!\n");

  return 0;
}
