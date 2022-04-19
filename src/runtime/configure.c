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

pt_queue_def_t const pt_queue_def[pt_queue_N] = {
    {
        .name = "Reader->Analyzer",
        .size = 1000,
        .is_block = true,
    },
    {
        .name = "Analyzer->Printer",
        .size = 1000,
        .is_block = true,
    },
    {
        .name = "Logger",
        .size = 1000,
        .is_block = true,
    },
};