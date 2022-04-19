#include "analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *analyzer_func(void *vargp) {
  (void)vargp;
//   while (1) {
    printf("Hello analyzer\n");
    sleep(10);
//   }

  return NULL;
}