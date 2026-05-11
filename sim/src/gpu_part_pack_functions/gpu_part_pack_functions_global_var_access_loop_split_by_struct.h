/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2025 Abouzied M. A. Nasar (abouzied.nasar@manchester.ac.uk)
 *                    Mladen Ivkovic (mladen.ivkovic@durham.ac.uk)
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
#pragma once

#ifndef USE_LOOP_SPLIT_BY_STRUCT
#error "How did we get here?"
#endif


/**
 * @file gpu_part_pack_functions_global_var_access_loop_split_by_struct.h
 * @brief Functions related to packing and unpacking particles to/from a cell.
 * Uses global variable for access in getters/setters. Loops are split by
 * underlying struct distribution.
 */

#include "active.h"
#include "cell.h"
#include "gpu_part_structs.h"
#include "hydro_part.h"

#include <math.h>

/* Apparently redundant declaration */
/* extern struct hydro_part_arrays global_hydro_part_arrays; */

/**
 * @brief Unpacks the density data from GPU buffers into cell's particle arrays
 *
 * @param c the #cell
 * @param parts_buffer the "receive" buffer to copy from
 * @param unpack_ind the index in parts_buffer to start copying from
 * @param count number of particles to unpack
 * @param e the #engine
 */
__attribute__((always_inline)) INLINE static void gpu_unpack_part_density(
    struct cell* restrict c,
    const struct gpu_part_recv_d* restrict parts_buffer, const int unpack_ind,
    const int count, const struct engine* restrict e) {

  const ptrdiff_t first =
      c->hydro.part_arrs._part - global_hydro_part_arrays._part;
  const struct gpu_part_recv_d* restrict parts_recv = &parts_buffer[unpack_ind];

  /* All three of them use a common `struct density_unpack`
   * The only difference is where `rho` is stored. It's always
   * in a different struct from `struct density_unpack`, but
   * that's the only other struct we're unpacking into, so
   * same loop split applies to all three of them. */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct gradient pack for SPHENIX_PACK_GRADIENT_PARTICLE */
    /* struct force pack for SPHENIX_PACK_FORCE_PARTICLE */
    /* struct force_gradient_pack_shared for SPHENIX_PACK_SHARED_PARTICLE */

    if (!part_is_active_global(pi, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho = part_get_rho_global(pi) + pr.rho_rhodh_wcount_wcount_dh.x;
    part_set_rho_global(pi, rho);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct density_unpack */

    if (!part_is_active_global(pi, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho_dh = part_get_rho_dh_global(pi) + pr.rho_rhodh_wcount_wcount_dh.y;
    part_set_rho_dh_global(pi, rho_dh);

    float wcount = part_get_wcount_global(pi) + pr.rho_rhodh_wcount_wcount_dh.z;
    part_set_wcount_global(pi, wcount);

    float wcount_dh =
        part_get_wcount_dh_global(pi) + pr.rho_rhodh_wcount_wcount_dh.w;
    part_set_wcount_dh_global(pi, wcount_dh);

    float* rot_v = part_get_rot_v_global(pi);
    rot_v[0] += pr.rot_vx_div_v.x;
    rot_v[1] += pr.rot_vx_div_v.y;
    rot_v[2] += pr.rot_vx_div_v.z;

    float div_v = part_get_div_v_global(pi) + pr.rot_vx_div_v.w;
    part_set_div_v_global(pi, div_v);
  }
}

/**
 * @brief Unpacks the gradient data from GPU buffers into cell's particle arrays
 *
 * @param c the #cell
 * @param parts_buffer the "receive" buffer to copy from
 * @param unpack_ind the index in parts_buffer to start copying from
 * @param count number of particles to unpack
 * @param e the #engine
 */
__attribute__((always_inline)) INLINE static void gpu_unpack_part_gradient(
    struct cell* restrict c,
    const struct gpu_part_recv_g* restrict parts_buffer, const int unpack_ind,
    const int count, const struct engine* e) {

  const ptrdiff_t first =
      c->hydro.part_arrs._part - global_hydro_part_arrays._part;
  const struct gpu_part_recv_g* restrict parts_recv = &parts_buffer[unpack_ind];

  /* All three variations (gradient, force, shared)
   * store all these variables in struct gradient_pack */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct gradient_pack */

    if (!part_is_active_global(pi, e)) continue;

    struct gpu_part_recv_g pr = parts_recv[i];

    float avisc_old = part_get_alpha_visc_max_ngb_global(pi);
    float avisc = fmaxf(avisc_old, pr.aviscmax_vsig_lapu.x);
    part_set_alpha_visc_max_ngb_global(pi, avisc);

    float vsig = fmaxf(pr.aviscmax_vsig_lapu.y, part_get_v_sig_global(pi));
    part_set_v_sig_global(pi, vsig);

    float lu = pr.aviscmax_vsig_lapu.z + part_get_laplace_u_global(pi);
    part_set_laplace_u_global(pi, lu);
  }
}

/**
 * @brief Unpacks the force data from GPU buffers into cell's particle arrays
 *
 * @param c the #cell
 * @param parts_buffer the "receive" buffer to copy from
 * @param unpack_ind the index in parts_buffer to start copying from
 * @param count number of particles to unpack
 * @param e the #engine
 */
__attribute__((always_inline)) INLINE static void gpu_unpack_part_force(
    struct cell* restrict c,
    const struct gpu_part_recv_f* restrict parts_buffer, const int unpack_ind,
    const int count, const struct engine* e) {

  const ptrdiff_t first =
      c->hydro.part_arrs._part - global_hydro_part_arrays._part;
  const struct gpu_part_recv_f* restrict parts_recv = &parts_buffer[unpack_ind];

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_unpack */

    if (!part_is_active_global(pi, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    float* a = part_get_a_hydro_global(pi);
    a[0] += pr.a_hydro.x;
    a[1] += pr.a_hydro.y;
    a[2] += pr.a_hydro.z;

    float u_dt = pr.udt_hdt_minngbtb.x + part_get_u_dt_global(pi);
    part_set_u_dt_global(pi, u_dt);

    float h_dt = pr.udt_hdt_minngbtb.y + part_get_h_dt_global(pi);
    part_set_h_dt_global(pi, h_dt);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    if (!part_is_active_global(pi, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    timebin_t mintbin = (timebin_t)(pr.udt_hdt_minngbtb.z + 0.5f);
    part_set_timestep_limiter_min_ngb_time_bin_global(pi, mintbin);
  }
}

/**
 * @brief Packs the cell particle data for density interactions into the
 * CPU-side buffers.
 *
 * @param c the #cell
 * @param parts_buffer the buffer to pack into
 * @param pack_ind the first free index in the buffer arrays to copy data into
 * @param shift periodic boundary shift
 * @param cjstart start index of cell cj's particles (which cell ci is to be
 * interacted with) in buffer
 * @param cjend end index of cell cj's particles (which cell ci is to be
 * interacted with) in buffer
 */
__attribute__((always_inline)) INLINE static void gpu_pack_part_density(
    const struct cell* restrict c,
    struct gpu_part_send_d* restrict parts_buffer, const int pack_ind,
    const double shift[3], const int cjstart, const int cjend) {

  /* Grab handles */
  const int count = c->hydro.count;
  const ptrdiff_t first =
      c->hydro.part_arrs._part - global_hydro_part_arrays._part;
  struct gpu_part_send_d* restrict ps = &parts_buffer[pack_ind];

  /* All three of them use a common `struct x_h_v_m`. */
  /* force-pack-type also splits data by type. */

#if defined(SPHENIX_PACK_FORCE_TYPE_PARTICLE) || defined(SPHENIX_PACK_GRADIENT_TYPE_PARTICLE) || defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m_double */

    const double* x = part_get_const_x_global(pi);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;

    ps[i].x_h.w = part_get_h_global(pi);

    const float* v = part_get_const_v_global(pi);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_global(pi);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#else
  /* every other pack-* type */

#ifdef VECTORIZE
#pragma omp simd
 #endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m */

    const double* x = part_get_const_x_global(pi);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_global(pi);

    const float* v = part_get_const_v_global(pi);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_global(pi);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }
#endif
}

/**
 * @brief Packs the cell particle data for gradient interactions into
 * the CPU-side buffers.
 *
 * @param ci the #cell
 * @param parts_buffer the buffer to pack into
 * @param pack_ind the first free index in the buffer arrays to copy data into
 * @param shift periodic boundary shift
 * @param cjstart start index of cell cj's particles (which cell ci is to be
 * interacted with) in buffer
 * @param cjend end index of cell cj's particles (which cell ci is to be
 * interacted with) in buffer
 */
__attribute__((always_inline)) INLINE static void gpu_pack_part_gradient(
    const struct cell* restrict ci,
    struct gpu_part_send_g* restrict parts_buffer, const int pack_ind,
    const double shift[3], const int cjstart, const int cjend) {

  /* Grab handles */
  const ptrdiff_t first =
      ci->hydro.part_arrs._part - global_hydro_part_arrays._part;
  const int count = ci->hydro.count;
  struct gpu_part_send_g* restrict ps = &parts_buffer[pack_ind];

  /* All three of them use a common `struct x_h_v_m`, but
   * but force-type additionally splits by type as well.
   * remaining variables are split among different structs. */

#if defined(SPHENIX_PACK_FORCE_TYPE_PARTICLE) || defined(SPHENIX_PACK_GRADIENT_TYPE_PARTICLE) || defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m_double */

    const double* x = part_get_const_x_global(pi);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m */

    ps[i].x_h.w = part_get_h_global(pi);

    const float* v = part_get_const_v_global(pi);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_global(pi);
  }

#else

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m */

    const double* x = part_get_const_x_global(pi);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_global(pi);

    const float* v = part_get_const_v_global(pi);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_global(pi);
  }
#endif

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || defined(SPHENIX_PACK_GRADIENT_TYPE_PARTICLE)
#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct gradient_pack */

    ps[i].u_rho_c_aviscmax.x = part_get_u_global(pi);
    ps[i].u_rho_c_aviscmax.y = part_get_rho_global(pi);
    ps[i].u_rho_c_aviscmax.z = part_get_soundspeed_global(pi);
    ps[i].u_rho_c_aviscmax.w = part_get_alpha_visc_max_ngb_global(pi);

    ps[i].avisc_vsig_lapu.x = part_get_alpha_av_global(pi);
    ps[i].avisc_vsig_lapu.y = part_get_v_sig_global(pi);
    ps[i].avisc_vsig_lapu.z = part_get_laplace_u_global(pi);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SPHENIX_PACK_FORCE_PARTICLE) || defined(SPHENIX_PACK_FORCE_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_gradient_share_pack */

    ps[i].u_rho_c_avisc.x = part_get_u_global(pi);
    ps[i].u_rho_c_avisc.y = part_get_rho_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_global(pi);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct gradient_pack */

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb_global(pi);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig_global(pi);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u_global(pi);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SPHENIX_PACK_SHARED_PARTICLE) || defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_gradient_share_pack */

    ps[i].u_rho_c_avisc.x = part_get_u_global(pi);
    ps[i].u_rho_c_avisc.y = part_get_rho_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */
    ps[i].u_rho_c_avisc.x = part_get_alpha_av_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct gradient_pack */

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb_global(pi);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig_global(pi);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u_global(pi);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#else
  /* Should be only SPHENIX_PACK_GRADIENT_PARTICLE,
   * SPHENIX_PACK_FORCE_PARTICLE, or SPHENIX_PACK_SHARED_PARTICLE */
#error "how did we get here...?"
#endif
}

/**
 * @brief Packs the cell particle data for force interactions into the
 * CPU-side buffers.
 *
 * @param ci the #cell
 * @param parts_buffer the buffer to pack into
 * @param pack_ind the first free index in the buffer arrays to copy data into
 * @param shift periodic boundary shift
 * @param cjstart start index of cell cj's particles (which cell ci is to be
 * interacted with) in buffer
 * @param cjend end index of cell cj's particles (which cell ci is to be
 * interacted with) in buffer
 */
__attribute__((always_inline)) INLINE static void gpu_pack_part_force(
    const struct cell* restrict ci,
    struct gpu_part_send_f* restrict parts_buffer, const int pack_ind,
    const double shift[3], const int cjstart, const int cjend) {

  const int count = ci->hydro.count;
  const ptrdiff_t first =
      ci->hydro.part_arrs._part - global_hydro_part_arrays._part;
  struct gpu_part_send_f* restrict ps = &parts_buffer[pack_ind];

  /* All three of them use a common `struct x_h_v_m`, but
   * force-type splits it among data type.
   * remaining variables are split among different structs. */

#if defined(SPHENIX_PACK_FORCE_TYPE_PARTICLE) || defined(SPHENIX_PACK_GRADIENT_TYPE_PARTICLE) || defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m */

    const double* x = part_get_const_x_global(pi);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m */

    ps[i].x_h.w = part_get_h_global(pi);

    const float* v = part_get_const_v_global(pi);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_global(pi);
  }


#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct x_h_v_m */

    const double* x = part_get_const_x_global(pi);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_global(pi);

    const float* v = part_get_const_v_global(pi);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_global(pi);
  }
#endif

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct gradient_pack */

    ps[i].u_rho_c_avisc.x = part_get_u_global(pi);
    ps[i].u_rho_c_avisc.y = part_get_rho_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_global(pi);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_global(pi);
    ps[i].f_p_balsara_adiff.y = part_get_pressure_global(pi);
    ps[i].f_p_balsara_adiff.z = part_get_balsara_global(pi);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_global(pi);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_global(pi);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_global(pi);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#elif defined(SPHENIX_PACK_GRADIENT_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct gradient_pack */

    ps[i].u_rho_c_avisc.x = part_get_u_global(pi);
    ps[i].u_rho_c_avisc.y = part_get_rho_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_global(pi);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_global(pi);
    ps[i].f_p_balsara_adiff.y = part_get_pressure_global(pi);
    ps[i].f_p_balsara_adiff.z = part_get_balsara_global(pi);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_global(pi);
  }
#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_global(pi);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }


#elif defined(SPHENIX_PACK_FORCE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].u_rho_f_p.x = part_get_u_global(pi);
    ps[i].u_rho_f_p.y = part_get_rho_global(pi);
    ps[i].u_rho_f_p.z = part_get_f_gradh_global(pi);
    ps[i].u_rho_f_p.w = part_get_pressure_global(pi);

    ps[i].bals_c_avisc_adiff.x = part_get_balsara_global(pi);
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed_global(pi);
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av_global(pi);
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff_global(pi);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_global(pi);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_global(pi);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#elif defined(SPHENIX_PACK_FORCE_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].u_rho_f_p.x = part_get_u_global(pi);
    ps[i].u_rho_f_p.y = part_get_rho_global(pi);
    ps[i].u_rho_f_p.z = part_get_f_gradh_global(pi);
    ps[i].u_rho_f_p.w = part_get_pressure_global(pi);

    ps[i].bals_c_avisc_adiff.x = part_get_balsara_global(pi);
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed_global(pi);
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av_global(pi);
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack_timebin */

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack_limiter */

    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_global(pi);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#elif defined(SPHENIX_PACK_SHARED_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_gradient_pack_shared */

    ps[i].u_rho_c_avisc.x = part_get_u_global(pi);
    ps[i].u_rho_c_avisc.y = part_get_rho_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_alpha_av_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_global(pi);
    ps[i].f_p_balsara_adiff.y = part_get_pressure_global(pi);
    ps[i].f_p_balsara_adiff.z = part_get_balsara_global(pi);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_global(pi);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_global(pi);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_global(pi);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#elif defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_gradient_pack_shared */

    ps[i].u_rho_c_avisc.x = part_get_u_global(pi);
    ps[i].u_rho_c_avisc.y = part_get_rho_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_global(pi);
    ps[i].u_rho_c_avisc.z = part_get_alpha_av_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_global(pi);
    ps[i].f_p_balsara_adiff.y = part_get_pressure_global(pi);
    ps[i].f_p_balsara_adiff.z = part_get_balsara_global(pi);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_global(pi);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (ptrdiff_t i = 0; i < count; i++) {
    ptrdiff_t pi = i + first;
    /* struct force_pack */

    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_global(pi);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }


#else
  /* should only be SPHENIX_PACK_SHARED_PARTICLE, SPHENIX_PACK_FORCE_PARTICLE,
   * or SPHENIX_PACK_GRADIENT_PARTICLE */
#error "how did we get here...?"
#endif
}
