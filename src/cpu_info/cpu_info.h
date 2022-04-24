#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PROCDIR
#define PROCDIR "/proc"
#endif

#ifndef PROCSTATFILE
#define PROCSTATFILE PROCDIR "/stat"
#endif

#ifndef PROC_LINE_LENGTH
#define PROC_LINE_LENGTH 4096
#endif

/* This subtraction is used by Linux / NetBSD / OpenBSD for calculation of CPU
 * usage items. */
static inline unsigned long long saturatingSub(unsigned long long a,
                                               unsigned long long b) {
  return a > b ? a - b : 0;
}

typedef struct CPUData_ {
  unsigned long long int totalTime;
  unsigned long long int userTime;
  unsigned long long int systemTime;
  unsigned long long int systemAllTime;
  unsigned long long int idleAllTime;
  unsigned long long int idleTime;
  unsigned long long int niceTime;
  unsigned long long int ioWaitTime;
  unsigned long long int irqTime;
  unsigned long long int softIrqTime;
  unsigned long long int stealTime;
  unsigned long long int guestTime;

  unsigned long long int totalPeriod;
  unsigned long long int userPeriod;
  unsigned long long int systemPeriod;
  unsigned long long int systemAllPeriod;
  unsigned long long int idleAllPeriod;
  unsigned long long int idlePeriod;
  unsigned long long int nicePeriod;
  unsigned long long int ioWaitPeriod;
  unsigned long long int irqPeriod;
  unsigned long long int softIrqPeriod;
  unsigned long long int stealPeriod;
  unsigned long long int guestPeriod;

  double frequency;

#ifdef HAVE_SENSORS_SENSORS_H
  double temperature;
#endif

  bool online;
} CPUData;

unsigned int get_cpu_count(void);
// static CPUData* getCPUsUsage(int cpus);
int getPercentageUsageOfCPUs(void);
char* get_CPU_raw_data(char* buffer, unsigned int cpus);
int get_percentage_usage_of_CPU(char* old, char* new, char* buffer);

#endif // PROCESS_H
