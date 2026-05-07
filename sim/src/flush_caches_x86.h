#pragma once

#include "config.h"
#include "inline.h"

#ifdef SWIFT_CACHE_FLUSH_X86
#include <x86intrin.h>

/**
 * Invalidate cache lines containing data of the array.
 *
 * https://stackoverflow.com/questions/68138772/c-function-to-flush-all-cache-lines-that-hold-an-array
 */
__attribute__((always_inline)) INLINE void flush_cache_x86(char* ptr,
                                                           size_t len) {
  const unsigned char cacheline = SWIFT_CACHE_LINE_SIZE;
  /* ptr_end modified to contain the last byte of its last cache line */
  char* ptr_end = (char*)(((size_t)ptr + len - 1) | (cacheline - 1));

  /* We run over the whole array and invalidate every possible cache line
   * containing any of its data */
  while (ptr <= ptr_end) {
    _mm_clflushopt(ptr);
    ptr += cacheline;
  }
  _mm_sfence();
}

/**
 * Invalidate cache lines containing data of the array without a final fence.
 *
 * https://stackoverflow.com/questions/68138772/c-function-to-flush-all-cache-lines-that-hold-an-array
 */
__attribute__((always_inline)) INLINE void flush_cache_x86_nofence(char* ptr,
                                                                   size_t len) {
  const unsigned char cacheline = SWIFT_CACHE_LINE_SIZE;
  /* ptr_end modified to contain the last byte of its last cache line */
  char* ptr_end = (char*)(((size_t)ptr + len - 1) | (cacheline - 1));

  /* We run over the whole array and invalidate every possible cache line
   * containing any of its data */
  while (ptr <= ptr_end) {
    _mm_clflushopt(ptr);
    ptr += cacheline;
  }
}

/**
 * Run a final sfence.
 *
 * https://stackoverflow.com/questions/68138772/c-function-to-flush-all-cache-lines-that-hold-an-array
 */
__attribute__((always_inline)) INLINE void flush_cache_x86_fence(void) {
  _mm_sfence();
}


#endif
