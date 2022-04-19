#include "watchdog.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *watchdog_func(void *vargp) {
  (void)vargp;
//   while (1) {
    printf("Hello watchdog\n");
    // sleep(10);
//   }

  return NULL;
}