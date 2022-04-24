#include "printer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime.h"

static char *trim_white_space(char *str) {
  char *end;

  // Trim leading space
  while (isspace((unsigned char)*str)) str++;

  if (*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

void *printer_func(void *vargp) {
  (void)vargp;

  int cpus = get_cpu_count();
  for (int j = 0; j < cpus; j++) {
    if (j == 0)
      printf("CPU\t\t");
    else
      printf("CPU%d\t\t", j);
  }
  printf("\n");

  while (1) {
    char *text = pt_queue_dequeue(pt_queue_analyzer_printer,
                                  sizeof(double) * cpus);  // four spaces

    if (text[0] == '\0') {
      continue;
    }

    for (int i = 0; i < cpus; i++) {
      char *data = trim_white_space(text + sizeof(double) * i);
      printf("%.*s%%\t\t", (int)sizeof(double), data);
    }
    printf("\n");
  }

  return NULL;
}