#include "printer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu_info.h"
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
  char *msg = malloc(sizeof(char) * 30);

  for (int j = 0; j < cpus; j++) {
    if (j == 0)
      printf("CPU\t\t");
    else
      printf("CPU%d\t\t", j);
  }
  printf("\n");

  while (pt_is_alive(pt_mutex_printer_alive)) {
    pt_set_alive(pt_mutex_printer_alive, true);

    char *text = pt_queue_dequeue(pt_queue_analyzer_printer,
                                  sizeof(double) * cpus);  // four spaces

    if (text[0] == '\0') {
      continue;
    }

    msg = "Printer prints...";
    pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);

    for (int i = 0; i < cpus; i++) {
      char *data = trim_white_space(text + sizeof(double) * i);
      printf("%.*s%%\t\t", (int)sizeof(double), data);
    }
    printf("\n");

    msg = "Printer ends printing.";
    pt_queue_enqueue(pt_queue_logger, msg, strlen(msg) + 1);
  }

  free(msg);
  return NULL;
}