/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2012 Pedro Gonnet (pedro.gonnet@durham.ac.uk)
 *                    Matthieu Schaller (schaller@strw.leidenuniv.nl)
 *               2016 John A. Regan (john.a.regan@durham.ac.uk)
 *                    Tom Theuns (tom.theuns@durham.ac.uk)
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
#ifndef SWIFT_TIMERS_H
#define SWIFT_TIMERS_H

/* Config parameters. */
#include "config.h"

/* Local includes. */
#include "atomic.h"
#include "cycle.h"
#include "inline.h"

/**
 * @brief The timers themselves.
 *
 * If you modify this list, be sure to change timers_names in timers.c as
 * well!
 **/
enum {
  timer_none = 0,
  timer_density_pack,
  timer_density_unpack,
  timer_gradient_pack,
  timer_gradient_unpack,
  timer_force_pack,
  timer_force_unpack,
  timer_count,
};

/* The timers. */
extern ticks timers[timer_count];

/* The timer names. */
extern const char *timers_names[];

/* Define the timer macros. */
/* #ifdef SWIFT_USE_TIMERS */
#define TIMER_TIC const ticks tic = getticks();
#define TIMER_TOC(t) timers_toc(t, tic)
#define TIMER_TIC2 const ticks tic2 = getticks();
#define TIMER_TOC2(t) timers_toc(t, tic2)
#define TIMER_TOC_LOCATION(t, loc) timers_toc_location(t, tic, loc);
INLINE static ticks timers_toc(unsigned int t, ticks tic) {
  const ticks d = (getticks() - tic);
  atomic_add(&timers[t], d);
  return d;
}
INLINE static ticks timers_toc_location(unsigned int t, ticks tic,
                                        ticks *location) {
  const ticks d = (getticks() - tic);
  atomic_add(&location[t], d);
  return d;
}
/* #else */
/* #define TIMER_TIC */
/* #define TIMER_TOC(t) (void)0 */
/* #define TIMER_TIC2 */
/* #define TIMER_TOC2(t) (void)0 */
/* #endif */

/* Function prototypes. */
void timers_reset_all(void);
void timers_reset(ticks *timer);
void timing_reset_time(double *time);
void timers_open_file(int rank);
void timers_close_file(void);
void timers_print_file(int step);
void print_timers(const ticks timers_arr[timer_count],
                  const double timing_log_arr[timer_count]);

#endif /* SWIFT_TIMERS_H */
