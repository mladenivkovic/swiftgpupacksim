/**
 * Functions related to flushing caches.
 */

#pragma once

#include "config.h"
#include "error.h"
#include "hydro_part.h"


/**
 * @brief Initialise a big array of meaningless floats.
 *
 * @param garbage: Array of garbage data to be allocated.
 * @param n_garbage: (out) number of elements in garbage array
 * @param no_cache_flush: If true, do not flush caches.
 * @param my_thread_id: Current omp thread id
 * @param nr_threads: Total number of threads in this run
 * @param verbose: Are we talkative?
 */
__attribute__((always_inline)) inline float* init_cache_flush(int* n_garbage, char no_cache_flush, int my_thread_id, int nr_threads, char verbose) {
#ifdef SWIFT_CACHE_FLUSH_ARRAY
  const int n = no_cache_flush ? 0 : 500000;
#elif defined SWIFT_CACHE_FLUSH_BIG_ARRAY
  const int n = no_cache_flush ? 0 : 2500000;
#elif defined SWIFT_CACHE_FLUSH_X86
  const int n = 0;
#else
#error "Unknown cache flushing mechanism"
#endif

  /* Return value of n */
  *n_garbage = n;

  /* Alloc a few MB of data and fill them with garbage to flush caches. */
  if (verbose) {
    if (my_thread_id == 0) {
      message("Allocating garbage array: estimated %g MB total",
          ((unsigned long)nr_threads * n * sizeof(double)) * 1e-6);
    }
#ifdef SWIFT_DEBUG_CHECKS
    message("Thread %d: Allocating garbage array: %g MB",
        my_thread_id, n * sizeof(float) * 1e-6);
#endif
  }

  float* garbage = malloc(n * sizeof(float));
  return garbage;
}




/**
 * Flush the caches by performing meaningless operations on a large array with
 * size O(MB)
 *
 * @param garbage A large array O(MB) to fill out with garbage data to
 * flush the caches after each op
 * @param n_garbage: size of garbage array
 * @param part_data: Struct holding pointers to all particle data
 * @param n_parts: total number of particles in part_data
 *
 * @return garbage_sum Some garbage float; Needed to prevent compiler from
 * optimising out a big loop doing nothing but flushing caches
 */
float flush_cache(float* garbage, int n_garbage,
 struct hydro_part_arrays* part_data, int n_parts
    ) {

#if defined(SWIFT_CACHE_FLUSH_ARRAY) || defined(SWIFT_CACHE_FLUSH_BIG_ARRAY)

  /* Use "poor man's cache flushing" */
  float sum = 0.;
  for (int i = 0; i < n_garbage; i++) {
    garbage[i] += 2. * garbage[i] - 13.;
    sum += garbage[i];
  }
  return sum;

#elif defined(SWIFT_CACHE_FLUSH_X86)
  /* Use x86 intinsics for cache flushing */



#else
#error "Unknown cache flushing mechanism"
#endif


}




