#include "printer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *printer_func(void *vargp) {
  (void)vargp;
//   while (1) {
    printf("Hello printer\n");
    // sleep(10);
//   }

  return NULL;
}