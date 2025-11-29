/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2012 Pedro Gonnet (pedro.gonnet@durham.ac.uk),
 *                    Matthieu Schaller (schaller@strw.leidenuniv.nl).
 *               2015 Peter W. Draper (p.w.draper@durham.ac.uk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#ifndef SWIFT_ERROR_H
#define SWIFT_ERROR_H

#include "inline.h"

/* Some standard headers. */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((always_inline)) INLINE static void omp_abort(void) {
  int threadid = omp_get_thread_num();
  int in_parallel = omp_in_parallel();
  fprintf(stderr, "aborting omp thread=%d in_parallel=%d\n", threadid,
          in_parallel);
  fflush(stderr);
#pragma omp error at(execution) severity(fatal) message("aborting omp")
}

/* #define swift_abort(errcode) abort() */
#define swift_abort(errcode) omp_abort();

/**
 * @brief Error macro. Prints the message given in argument and aborts.
 *
 */
#define error(s, ...)                                                        \
  ({                                                                         \
    fflush(stdout);                                                          \
    fprintf(stderr, "%s:%s():%i: " s "\n", __FILE__, __FUNCTION__, __LINE__, \
            ##__VA_ARGS__);                                                  \
    fflush(stderr);                                                          \
    swift_abort(1);                                                          \
  })

/**
 * @brief Macro to print a localized message with variable arguments.
 *
 */
#define message(s, ...)                                 \
  ({                                                    \
    printf("%s: " s "\n", __FUNCTION__, ##__VA_ARGS__); \
    fflush(stdout);                                     \
  })

/**
 * @brief Macro to print a localized warning message with variable arguments.
 *
 * Same as message(), but this version prints to the standard error.
 *
 */
#define warning(s, ...)                                                     \
  ({                                                                        \
    fprintf(stderr, "%s:%s():%i: WARNING: " s "\n", __FILE__, __FUNCTION__, \
            __LINE__, ##__VA_ARGS__);                                       \
    fflush(stdout);                                                         \
  })

/**
 * @brief Assertion macro compatible with MPI
 *
 */
#define swift_assert(expr)                                                    \
  ({                                                                          \
    if (!(expr)) {                                                            \
      fflush(stdout);                                                         \
      fprintf(stderr, "%s:%s():%i: FAILED ASSERTION: " #expr " \n", __FILE__, \
              __FUNCTION__, __LINE__);                                        \
      fflush(stderr);                                                         \
      swift_abort(1);                                                         \
    }                                                                         \
  })

#endif /* SWIFT_ERROR_H */
