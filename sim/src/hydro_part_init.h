/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2026 Mladen Ivkovic (mladen.ivkovic@durham.ac.uk)
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
#ifndef SWIFT_HYDRO_PART_INIT_H
#define SWIFT_HYDRO_PART_INIT_H


/**
 * @file src/hydro_part_init.h
 * @brief Contains functions initialising (junk) particle values.
 * */

/* Config parameters. */
#include "../config.h"
#include "hydro_part.h"
#include "inline.h"


static __attribute__((always_inline)) INLINE void hydro_part_init(struct hydro_part_arrays* part_data, long long pid) {

#if defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)

  /* These realisations have a single particle struct. They don't need
   * special handling here. */

#elif defined(SPHENIX_SOA_PARTICLE) ||         \
    defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
    defined(SPHENIX_PACK_SHARED_PARTICLE)

  /* Split structs need initialisation of array pointer + their index in that
   * array */
  part_data._part[pid]._cell_offset = pid;
  part_data._part[pid]._cell_part_arrays = part_data;
#else
#error "particle handling for this memory layout not implemented"
#endif

  struct part* restrict p = &part_data->_part[pid];

  part_set_id(p, pid);
  part_set_gpart(p, NULL);
  part_set_x_ind(p, 0, 1.);
  part_set_x_ind(p, 1, 2.);
  part_set_x_ind(p, 2, 3.);
  part_set_a_hydro_ind(p, 0, 4.f);
  part_set_a_hydro_ind(p, 1, 5.f);
  part_set_a_hydro_ind(p, 2, 6.f);
  part_set_mass(p, 7.f);
  part_set_h(p, 8.f);
  part_set_u(p, 9.f);
  part_set_u_dt(p, 10.f);
  part_set_rho(p, 11.f);
  part_set_div_v(p, 12.f);
  part_set_div_v_dt(p, 13.f);
  part_set_div_v_previous_step(p, 14.f);
  part_set_alpha_av(p, 15.f);
  part_set_v_sig(p, 16.f);
  part_set_laplace_u(p, 17.f);
  part_set_alpha_diff(p, 18.f);
  part_set_wcount(p, 19.f);
  part_set_wcount_dh(p, 20.f);
  part_set_rho_dh(p, 21.f);
  part_set_rot_v_ind(p, 0, 22.f);
  part_set_rot_v_ind(p, 1, 23.f);
  part_set_rot_v_ind(p, 2, 24.f);
  part_set_f_gradh(p, 25.f);
  part_set_pressure(p, 26.f);
  part_set_soundspeed(p, 27.f);
  part_set_h_dt(p, 28.f);
  part_set_balsara(p, 29.f);
  part_set_alpha_visc_max_ngb(p, 30.f);
  part_set_depth_h(p, 1);
  part_set_time_bin(p, 2);
}

#endif
