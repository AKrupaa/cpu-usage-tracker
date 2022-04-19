#include "configure.h"

#include <stddef.h>

#include "analyzer.h"
#include "logger.h"
#include "printer.h"
#include "reader.h"
#include "runtime.h"
#include "watchdog.h"

pt_thread_def_t const pt_thread_def[pt_thread_N] = {
    {.name = "Reader", .attr = NULL, .func = reader_func, .arg = NULL},
    {.name = "Analyzer", .attr = NULL, .func = analyzer_func, .arg = NULL},
    {.name = "Printer", .attr = NULL, .func = printer_func, .arg = NULL},
    {.name = "Watchdog", .attr = NULL, .func = watchdog_func, .arg = NULL},
    {.name = "Logger", .attr = NULL, .func = logger_func, .arg = NULL},
};