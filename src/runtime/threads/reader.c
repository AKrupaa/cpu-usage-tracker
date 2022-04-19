#include "reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *reader_func(void *vargp) {
  (void)vargp;
//   while (1) {
    printf("Hello reader\n");
    // sleep(10);
//   }

  return NULL;
}