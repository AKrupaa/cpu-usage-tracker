#include "cpu_info.h"

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int get_cpu_count() {
  return get_nprocs() + 1;  // +1 for cpu
}

char* get_CPU_raw_data(char* buffer, int cpus) {
  FILE* file = fopen(PROCSTATFILE, "r");
  if (file == NULL) {
    printf("Error: %s\n", strerror(errno));
  }

  char line[PROC_LINE_LENGTH + 1];
  int cpu = 0;

  while (cpu < cpus) {
    char* ok = fgets(line, PROC_LINE_LENGTH, file);
    if (!ok) buffer[0] = '\0';

    if (strstr(line, "cpu") != NULL) {
      // strcat(buffer + (PROC_LINE_LENGTH + 1) * cpu, line);
      memcpy(buffer + (PROC_LINE_LENGTH + 1) * cpu, line, PROC_LINE_LENGTH + 1);
    }

    cpu++;
  }

  fclose(file);

  return buffer;
}

static CPUData* calulcate_CPU_usage(char* buffer) {
  int cpus = get_cpu_count();
  CPUData* cpuData = malloc(sizeof(CPUData) * cpus);

  assert(cpus > 0);
  for (int i = 0; i < cpus; i++) {
    // char buffer[PROC_LINE_LENGTH + 1];
    unsigned long long int usertime, nicetime, systemtime, idletime;
    unsigned long long int ioWait, irq, softIrq, steal, guest, guestnice;
    ioWait = irq = softIrq = steal = guest = guestnice = 0;
    // Depending on your kernel version,
    // 5, 7, 8 or 9 of these fields will be set.
    // The rest will remain at zero.
    // char* ok = fgets(buffer, PROC_LINE_LENGTH, file);
    // if (!ok) buffer[0] = '\0';
    if (i == 0) {
      char* test = buffer + (PROC_LINE_LENGTH + 1) * i;
      (void)sscanf(test,
                   "cpu  %16llu %16llu %16llu %16llu %16llu %16llu %16llu "
                   "%16llu %16llu %16llu",
                   &usertime, &nicetime, &systemtime, &idletime, &ioWait, &irq,
                   &softIrq, &steal, &guest, &guestnice);
    } else {
      int cpuid;
      char* test = buffer + (PROC_LINE_LENGTH + 1) * i;
      (void)sscanf(test,
                   "cpu%4d %16llu %16llu %16llu %16llu %16llu %16llu %16llu "
                   "%16llu %16llu %16llu",
                   &cpuid, &usertime, &nicetime, &systemtime, &idletime,
                   &ioWait, &irq, &softIrq, &steal, &guest, &guestnice);
      assert(cpuid == i - 1);
    }

    // Guest time is already accounted in usertime
    usertime = usertime - guest;
    nicetime = nicetime - guestnice;
    // Fields existing on kernels >= 2.6
    // (and RHEL's patched kernel 2.4...)
    unsigned long long int idlealltime = idletime + ioWait;
    unsigned long long int systemalltime = systemtime + irq + softIrq;
    unsigned long long int virtalltime = guest + guestnice;
    unsigned long long int totaltime =
        usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;
    // Since we do a subtraction (usertime - guest) and cputime64_to_clock_t()
    // used in /proc/stat rounds down numbers, it can lead to a case where the
    // integer overflow.
    cpuData[i].userPeriod = saturatingSub(usertime, cpuData[i].userTime);
    cpuData[i].nicePeriod = saturatingSub(nicetime, cpuData[i].niceTime);
    cpuData[i].systemPeriod = saturatingSub(systemtime, cpuData[i].systemTime);
    cpuData[i].systemAllPeriod =
        saturatingSub(systemalltime, cpuData[i].systemAllTime);
    cpuData[i].idleAllPeriod =
        saturatingSub(idlealltime, cpuData[i].idleAllTime);
    cpuData[i].idlePeriod = saturatingSub(idletime, cpuData[i].idleTime);
    cpuData[i].ioWaitPeriod = saturatingSub(ioWait, cpuData[i].ioWaitTime);
    cpuData[i].irqPeriod = saturatingSub(irq, cpuData[i].irqTime);
    cpuData[i].softIrqPeriod = saturatingSub(softIrq, cpuData[i].softIrqTime);
    cpuData[i].stealPeriod = saturatingSub(steal, cpuData[i].stealTime);
    cpuData[i].guestPeriod = saturatingSub(virtalltime, cpuData[i].guestTime);
    cpuData[i].totalPeriod = saturatingSub(totaltime, cpuData[i].totalTime);
    cpuData[i].userTime = usertime;
    cpuData[i].niceTime = nicetime;
    cpuData[i].systemTime = systemtime;
    cpuData[i].systemAllTime = systemalltime;
    cpuData[i].idleAllTime = idlealltime;
    cpuData[i].idleTime = idletime;
    cpuData[i].ioWaitTime = ioWait;
    cpuData[i].irqTime = irq;
    cpuData[i].softIrqTime = softIrq;
    cpuData[i].stealTime = steal;
    cpuData[i].guestTime = virtalltime;
    cpuData[i].totalTime = totaltime;
  }

  return cpuData;
}

int get_percentage_usage_of_CPU(char* old, char* new, char* buffer) {
  int cpus = get_cpu_count();

  CPUData* prevCPU = calulcate_CPU_usage(old);
  CPUData* currCPU = calulcate_CPU_usage(new);

  for (int i = 0; i < cpus; i++) {
    unsigned long long int prevIdle =
        prevCPU[i].idleTime + prevCPU[i].ioWaitTime;
    unsigned long long int idle = currCPU[i].idleTime + currCPU[i].ioWaitTime;
    unsigned long long int prevNonIdle =
        prevCPU[i].userTime + prevCPU[i].niceTime + prevCPU[i].systemTime +
        prevCPU[i].irqTime + prevCPU[i].softIrqTime + prevCPU[i].stealTime;

    unsigned long long int nonIdle =
        currCPU[i].userTime + currCPU[i].niceTime + currCPU[i].systemTime +
        currCPU[i].irqTime + currCPU[i].softIrqTime + currCPU[i].stealTime;

    unsigned long long int prevTotal = prevIdle + prevNonIdle;
    unsigned long long int total = idle + nonIdle;

    unsigned long long int totalDiff = total - prevTotal;
    unsigned long long int idleDiff = idle - prevIdle;

    double cpuUsagePercentage =
        (double)((totalDiff - idleDiff) * 100) / totalDiff;

    sprintf(buffer + sizeof(double) * i, "%5.2f ", cpuUsagePercentage);
  }

  free(prevCPU);
  free(currCPU);

  return 0;
}
