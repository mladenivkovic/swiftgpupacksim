#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <stddef.h>

#include "barrier.h"

/* Function type for mappings. */
typedef void (*threadpool_map_function)(void *map_data, int num_elements,
                                        void *extra_data);


/* Data of a threadpool. */
struct threadpool {

  /* The threads themselves. */
  pthread_t *threads;

  /* This is where threads go to rest. */
  swift_barrier_t wait_barrier;
  swift_barrier_t run_barrier;

  /* Current map data and count. */
  void *map_data, *map_extra_data;
  volatile size_t map_data_count, map_data_size, map_data_stride;
  volatile ptrdiff_t map_data_chunk;
  volatile threadpool_map_function map_function;

  /* Number of threads in this pool. */
  int num_threads;

  /* Counter for the number of threads that are done. */
  volatile int num_threads_running;

#ifdef SWIFT_DEBUG_THREADPOOL
  struct mapper_log *logs;
#endif
};

#endif
