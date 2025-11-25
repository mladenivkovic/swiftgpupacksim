/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2020 Mladen Ivkovic (mladen.ivkovic@hotmail.com)
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
#ifndef SWIFT_RT_STRUCT_H
#define SWIFT_RT_STRUCT_H

/**
 * @file src/rt_struct.h
 * @brief Branches between the different radiative transfer structs.
 */

/* Config parameters. */
#include "timeline.h"

/* Import the right RT struct definition */

/* Define a struct to contain all RT sub-cycling related
 * timestepping variables here. These variables need to be
 * identical for every scheme and users should never touch
 * them anyway, so hide them here. */

/*! empty placeholder for RT timestepping data. */
struct rt_timestepping_data {
  union {
    /*! Time-bin this particle uses for RT interactions */
    timebin_t _time_bin;

    /*! Minimal time-bin across all neighbours */
    timebin_t _min_ngb_time_bin;
  };
};

static __attribute__((always_inline)) INLINE timebin_t
rt_timestepping_data_get_time_bin(
    const struct rt_timestepping_data* restrict rtd) {
  return rtd->_time_bin;
}
static __attribute__((always_inline)) INLINE void
rt_timestepping_data_set_time_bin(struct rt_timestepping_data* restrict rtd,
                                  const timebin_t time_bin) {
  rtd->_time_bin = time_bin;
}

static __attribute__((always_inline)) INLINE timebin_t
rt_timestepping_data_get_min_ngb_time_bin(
    const struct rt_timestepping_data* restrict rtd) {
  return rtd->_min_ngb_time_bin;
}
static __attribute__((always_inline)) INLINE void
rt_timestepping_data_set_min_ngb_time_bin(
    struct rt_timestepping_data* restrict rtd,
    const timebin_t min_ngb_time_bin) {
  rtd->_min_ngb_time_bin = min_ngb_time_bin;
}

#endif /* SWIFT_RT_STRUCT_H */
