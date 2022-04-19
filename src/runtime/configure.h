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

typedef enum pt_queue_ { pt_queue_0 = 0 } pt_queue_t;

#endif /* CONFIGURE_H */