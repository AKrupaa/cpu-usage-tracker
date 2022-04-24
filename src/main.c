#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "runtime/runtime.h"

int main(int argc, char const* argv[]) {
  /* unused parameter */
  (void)argc;
  (void)argv;

  pt_runtime_init();
  pt_thread_join();

  return 0;
}
