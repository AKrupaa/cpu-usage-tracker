#ifndef CONFIGURE_H
#define CONFIGURE_H

typedef enum _pt_thread_ {
  pt_thread_0 = 0,
  pt_thread_Reader = pt_thread_0,
  pt_thread_Analzyer,
  pt_thread_Printer,
  pt_thread_Watchdog,
  pt_thread_Logger,
  pt_thread_N
} pt_thread_t;

typedef enum pt_queue_ {
  pt_queue_0 = 0,
  pt_queue_reader_analyzer = pt_queue_0,
  pt_queue_analyzer_printer,
  pt_queue_logger,
  pt_queue_N
} pt_queue_t;

typedef enum pt_mutex_ {
  // mutex for queues
  pt_mutex_queue_0 = 0,
  pt_mutex_reader_analyzer = pt_mutex_queue_0,
  pt_mutex_analyzer_printer,
  pt_mutex_logger,
  pt_mutex_queue_N,
  // mutex for threads
  pt_mutex_threads_alive_0 = pt_mutex_queue_N,
  pt_mutex_reader_alive = pt_mutex_threads_alive_0,
  pt_mutex_analyzer_alive,
  pt_mutex_printer_alive,
  pt_mutex_watchdog_alive,
  pt_mutex_logger_alive,
  pt_mutex_threads_alive_N,
  pt_mutex_N = pt_mutex_threads_alive_N,
} pt_mutex_t;

#endif /* CONFIGURE_H */