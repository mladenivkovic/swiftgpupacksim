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

/**
 * @file gpu_part_pack_functions_part_struct_access.h
 * @brief Functions related to packing and unpacking particles to/from a cell.
 * Uses particle struct for access in getters/setters
 */

#include "active.h"
#include "cell.h"
#include "gpu_part_structs.h"
#include "hydro_part.h"

#include <math.h>

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

  const struct gpu_part_recv_d* restrict parts_recv = &parts_buffer[unpack_ind];
  struct hydro_part_arrays* restrict pd = &c->hydro.part_arrs;

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* -------------------------------------------------- */
  /* Loops over all fields of a particle each iteration */
  /* -------------------------------------------------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho = part_get_rho_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.x;
    part_set_rho_explicit(pd, i, rho);

    float rho_dh = part_get_rho_dh_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.y;
    part_set_rho_dh_explicit(pd, i, rho_dh);

    float wcount = part_get_wcount_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.z;
    part_set_wcount_explicit(pd, i, wcount);

    float wcount_dh = part_get_wcount_dh_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.w;
    part_set_wcount_dh_explicit(pd, i, wcount_dh);

    float* rot_v = part_get_rot_v_explicit(pd, i);
    rot_v[0] += pr.rot_vx_div_v.x;
    rot_v[1] += pr.rot_vx_div_v.y;
    rot_v[2] += pr.rot_vx_div_v.z;

    float div_v = part_get_div_v_explicit(pd, i) + pr.rot_vx_div_v.w;
    part_set_div_v_explicit(pd, i, div_v);
  }
#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* ---------------------------------------- */
  /* Loops split by struct field distribution */
  /* ---------------------------------------- */

  /* All three of them use a common `struct density_unpack`
   * The only difference is where `rho` is stored. It's always
   * in a different struct from `struct density_unpack`, but
   * that's the only other struct we're unpacking into, so
   * same loop split applies to all three of them. */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient pack for SPHENIX_PACK_GRADIENT_PARTICLE */
    /* struct force pack for SPHENIX_PACK_FORCE_PARTICLE */
    /* struct force_gradient_pack_shared for SPHENIX_PACK_SHARED_PARTICLE */

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho = part_get_rho_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.x;
    part_set_rho_explicit(pd, i, rho);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct density_unpack */

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho_dh = part_get_rho_dh_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.y;
    part_set_rho_dh_explicit(pd, i, rho_dh);

    float wcount = part_get_wcount_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.z;
    part_set_wcount_explicit(pd, i, wcount);

    float wcount_dh = part_get_wcount_dh_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.w;
    part_set_wcount_dh_explicit(pd, i, wcount_dh);

    float* rot_v = part_get_rot_v_explicit(pd, i);
    rot_v[0] += pr.rot_vx_div_v.x;
    rot_v[1] += pr.rot_vx_div_v.y;
    rot_v[2] += pr.rot_vx_div_v.z;

    float div_v = part_get_div_v_explicit(pd, i) + pr.rot_vx_div_v.w;
    part_set_div_v_explicit(pd, i, div_v);
  }


#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* ---------- */
  /* SoA access */
  /* ---------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float rho = part_get_rho_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.x;
    part_set_rho_explicit(pd, i, rho);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];
    float rho_dh = part_get_rho_dh_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.y;
    part_set_rho_dh_explicit(pd, i, rho_dh);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float wcount = part_get_wcount_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.z;
    part_set_wcount_explicit(pd, i, wcount);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float wcount_dh = part_get_wcount_dh_explicit(pd, i) + pr.rho_rhodh_wcount_wcount_dh.w;
    part_set_wcount_dh_explicit(pd, i, wcount_dh);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float* rot_v = part_get_rot_v_explicit(pd, i);
    rot_v[0] += pr.rot_vx_div_v.x;
    rot_v[1] += pr.rot_vx_div_v.y;
    rot_v[2] += pr.rot_vx_div_v.z;
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float div_v = part_get_div_v_explicit(pd, i) + pr.rot_vx_div_v.w;
    part_set_div_v_explicit(pd, i, div_v);
  }
#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
#endif
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

  const struct gpu_part_recv_g* restrict parts_recv = &parts_buffer[unpack_ind];
  struct hydro_part_arrays* restrict pd = &c->hydro.part_arrs;

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* -------------------------------------------------- */
  /* Loops over all fields of a particle each iteration */
  /* -------------------------------------------------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_g pr = parts_recv[i];

    float avisc_old = part_get_alpha_visc_max_ngb_explicit(pd, i);
    float avisc = fmaxf(avisc_old, pr.aviscmax_vsig_lapu.x);
    part_set_alpha_visc_max_ngb_explicit(pd, i, avisc);

    float vsig = fmaxf(pr.aviscmax_vsig_lapu.y, part_get_v_sig_explicit(pd, i));
    part_set_v_sig_explicit(pd, i, vsig);

    float lu = pr.aviscmax_vsig_lapu.z + part_get_laplace_u_explicit(pd, i);
    part_set_laplace_u_explicit(pd, i, lu);
  }

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* ---------------------------------------- */
  /* Loops split by struct field distribution */
  /* ---------------------------------------- */

  /* All three variations (gradient, force, shared)
   * store all these variables in struct gradient_pack */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_g pr = parts_recv[i];

    float avisc_old = part_get_alpha_visc_max_ngb_explicit(pd, i);
    float avisc = fmaxf(avisc_old, pr.aviscmax_vsig_lapu.x);
    part_set_alpha_visc_max_ngb_explicit(pd, i, avisc);

    float vsig = fmaxf(pr.aviscmax_vsig_lapu.y, part_get_v_sig_explicit(pd, i));
    part_set_v_sig_explicit(pd, i, vsig);

    float lu = pr.aviscmax_vsig_lapu.z + part_get_laplace_u_explicit(pd, i);
    part_set_laplace_u_explicit(pd, i, lu);
  }

#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* ---------- */
  /* SoA access */
  /* ---------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_g pr = parts_recv[i];

    float avisc_old = part_get_alpha_visc_max_ngb_explicit(pd, i);
    float avisc = fmaxf(avisc_old, pr.aviscmax_vsig_lapu.x);
    part_set_alpha_visc_max_ngb_explicit(pd, i, avisc);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_g pr = parts_recv[i];

    float vsig = fmaxf(pr.aviscmax_vsig_lapu.y, part_get_v_sig_explicit(pd, i));
    part_set_v_sig_explicit(pd, i, vsig);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_g pr = parts_recv[i];

    float lu = pr.aviscmax_vsig_lapu.z + part_get_laplace_u_explicit(pd, i);
    part_set_laplace_u_explicit(pd, i, lu);
  }
#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
#endif
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

  const struct gpu_part_recv_f* restrict parts_recv = &parts_buffer[unpack_ind];
  struct hydro_part_arrays* restrict pd = &c->hydro.part_arrs;

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* -------------------------------------------------- */
  /* Loops over all fields of a particle each iteration */
  /* -------------------------------------------------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    float* a = part_get_a_hydro_explicit(pd, i);
    a[0] += pr.a_hydro.x;
    a[1] += pr.a_hydro.y;
    a[2] += pr.a_hydro.z;

    float u_dt = pr.udt_hdt_minngbtb.x + part_get_u_dt_explicit(pd, i);
    part_set_u_dt_explicit(pd, i, u_dt);

    float h_dt = pr.udt_hdt_minngbtb.y + part_get_h_dt_explicit(pd, i);
    part_set_h_dt_explicit(pd, i, h_dt);

    timebin_t mintbin = (timebin_t)(pr.udt_hdt_minngbtb.z + 0.5f);
    part_set_timestep_limiter_min_ngb_time_bin_explicit(pd, i, mintbin);
  }

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* ---------------------------------------- */
  /* Loops split by struct field distribution */
  /* ---------------------------------------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_unpack */

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    float* a = part_get_a_hydro_explicit(pd, i);
    a[0] += pr.a_hydro.x;
    a[1] += pr.a_hydro.y;
    a[2] += pr.a_hydro.z;

    float u_dt = pr.udt_hdt_minngbtb.x + part_get_u_dt_explicit(pd, i);
    part_set_u_dt_explicit(pd, i, u_dt);

    float h_dt = pr.udt_hdt_minngbtb.y + part_get_h_dt_explicit(pd, i);
    part_set_h_dt_explicit(pd, i, h_dt);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */

    if (!part_is_active_explicit(pd, i, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    timebin_t mintbin = (timebin_t)(pr.udt_hdt_minngbtb.z + 0.5f);
    part_set_timestep_limiter_min_ngb_time_bin_explicit(pd, i, mintbin);
  }


#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* ---------- */
  /* SoA access */
  /* ---------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];

    float* a = part_get_a_hydro_explicit(pd, i);
    a[0] += pr.a_hydro.x;
    a[1] += pr.a_hydro.y;
    a[2] += pr.a_hydro.z;
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];

    float u_dt = pr.udt_hdt_minngbtb.x + part_get_u_dt_explicit(pd, i);
    part_set_u_dt_explicit(pd, i, u_dt);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];
    float h_dt = pr.udt_hdt_minngbtb.y + part_get_h_dt_explicit(pd, i);

    part_set_h_dt_explicit(pd, i, h_dt);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    if (!part_is_active_explicit(pd, i, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];

    timebin_t mintbin = (timebin_t)(pr.udt_hdt_minngbtb.z + 0.5f);
    part_set_timestep_limiter_min_ngb_time_bin_explicit(pd, i, mintbin);
  }

#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
#endif
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
  const struct hydro_part_arrays* restrict pd = &c->hydro.part_arrs;
  struct gpu_part_send_d* restrict ps = &parts_buffer[pack_ind];

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* -------------------------------------------------- */
  /* Loops over all fields of a particle each iteration */
  /* -------------------------------------------------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {

    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_explicit(pd, i);

    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* ---------------------------------------- */
  /* Loops split by struct field distribution */
  /* ---------------------------------------- */

  /* All three of them use a common `struct x_h_v_m`. */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct x_h_v_m */

    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_explicit(pd, i);

    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* ---------- */
  /* SoA access */
  /* ---------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].x_h.w = part_get_h_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }
#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
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
  const int count = ci->hydro.count;
  const struct hydro_part_arrays* restrict pd = &ci->hydro.part_arrs;
  struct gpu_part_send_g* restrict ps = &parts_buffer[pack_ind];

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* -------------------------------------------------- */
  /* Loops over all fields of a particle each iteration */
  /* -------------------------------------------------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {

    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_explicit(pd, i);

    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);

#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_explicit(pd, i);

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb_explicit(pd, i);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig_explicit(pd, i);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u_explicit(pd, i);
#else
    ps[i].u_rho_c_aviscmax.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_aviscmax.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_aviscmax.z = part_get_soundspeed_explicit(pd, i);
    ps[i].u_rho_c_aviscmax.w = part_get_alpha_visc_max_ngb_explicit(pd, i);

    ps[i].avisc_vsig_lapu.x = part_get_alpha_av_explicit(pd, i);
    ps[i].avisc_vsig_lapu.y = part_get_v_sig_explicit(pd, i);
    ps[i].avisc_vsig_lapu.z = part_get_laplace_u_explicit(pd, i);
#endif

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* ---------------------------------------- */
  /* Loops split by struct field distribution */
  /* ---------------------------------------- */

  /* All three of them use a common `struct x_h_v_m`, but
   * remaining variables are split among different structs. */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct x_h_v_m */

    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_explicit(pd, i);

    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);
  }

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE)
#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    ps[i].u_rho_c_aviscmax.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_aviscmax.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_aviscmax.z = part_get_soundspeed_explicit(pd, i);
    ps[i].u_rho_c_aviscmax.w = part_get_alpha_visc_max_ngb_explicit(pd, i);

    ps[i].avisc_vsig_lapu.x = part_get_alpha_av_explicit(pd, i);
    ps[i].avisc_vsig_lapu.y = part_get_v_sig_explicit(pd, i);
    ps[i].avisc_vsig_lapu.z = part_get_laplace_u_explicit(pd, i);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SPHENIX_PACK_FORCE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_gradient_share_pack */

    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb_explicit(pd, i);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig_explicit(pd, i);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u_explicit(pd, i);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SPHENIX_PACK_SHARED_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_gradient_share_pack */

    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */
    ps[i].u_rho_c_avisc.x = part_get_alpha_av_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb_explicit(pd, i);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig_explicit(pd, i);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u_explicit(pd, i);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#else
  /* Should be only SPHENIX_PACK_GRADIENT_PARTICLE,
   * SPHENIX_PACK_FORCE_PARTICLE, or SPHENIX_PACK_SHARED_PARTICLE */
#pragma error "how did we get here...?"
#endif


#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* ---------- */
  /* SoA access */
  /* ---------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].x_h.w = part_get_h_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
#else
    ps[i].u_rho_c_aviscmax.x = part_get_u_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
#else
    ps[i].u_rho_c_aviscmax.y = part_get_rho_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
#else
    ps[i].u_rho_c_aviscmax.z = part_get_soundspeed_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_explicit(pd, i);
#else
    ps[i].u_rho_c_aviscmax.w = part_get_alpha_visc_max_ngb_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb_explicit(pd, i);
#else
    ps[i].avisc_vsig_lapu.x = part_get_alpha_av_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig_explicit(pd, i);
#else
    ps[i].avisc_vsig_lapu.y = part_get_v_sig_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u_explicit(pd, i);
#else
    ps[i].avisc_vsig_lapu.z = part_get_laplace_u_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }
#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
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
  const struct hydro_part_arrays* restrict pd = &ci->hydro.part_arrs;
  struct gpu_part_send_f* restrict ps = &parts_buffer[pack_ind];

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* -------------------------------------------------- */
  /* Loops over all fields of a particle each iteration */
  /* -------------------------------------------------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {

    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_explicit(pd, i);

    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_explicit(pd, i);

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_explicit(pd, i);
    ps[i].f_p_balsara_adiff.y = part_get_pressure_explicit(pd, i);
    ps[i].f_p_balsara_adiff.z = part_get_balsara_explicit(pd, i);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_explicit(pd, i);
#else
    ps[i].u_rho_f_p.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_f_p.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_f_p.z = part_get_f_gradh_explicit(pd, i);
    ps[i].u_rho_f_p.w = part_get_pressure_explicit(pd, i);

    ps[i].bals_c_avisc_adiff.x = part_get_balsara_explicit(pd, i);
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed_explicit(pd, i);
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av_explicit(pd, i);
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff_explicit(pd, i);
#endif

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_explicit(pd, i);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_explicit(pd, i);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* ---------------------------------------- */
  /* Loops split by struct field distribution */
  /* ---------------------------------------- */

  /* All three of them use a common `struct x_h_v_m`, but
   * remaining variables are split among different structs. */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct x_h_v_m */

    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h_explicit(pd, i);

    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);
  }

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_explicit(pd, i);
    ps[i].f_p_balsara_adiff.y = part_get_pressure_explicit(pd, i);
    ps[i].f_p_balsara_adiff.z = part_get_balsara_explicit(pd, i);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_explicit(pd, i);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_explicit(pd, i);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_explicit(pd, i);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#elif defined(SPHENIX_PACK_FORCE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */

    ps[i].u_rho_f_p.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_f_p.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_f_p.z = part_get_f_gradh_explicit(pd, i);
    ps[i].u_rho_f_p.w = part_get_pressure_explicit(pd, i);

    ps[i].bals_c_avisc_adiff.x = part_get_balsara_explicit(pd, i);
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed_explicit(pd, i);
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av_explicit(pd, i);
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff_explicit(pd, i);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_explicit(pd, i);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_explicit(pd, i);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#elif defined(SPHENIX_PACK_SHARED_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_gradient_pack_shared */

    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
    ps[i].u_rho_c_avisc.z = part_get_alpha_av_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_explicit(pd, i);
    ps[i].f_p_balsara_adiff.y = part_get_pressure_explicit(pd, i);
    ps[i].f_p_balsara_adiff.z = part_get_balsara_explicit(pd, i);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_explicit(pd, i);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_explicit(pd, i);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_explicit(pd, i);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }

#else
  /* should only be SPHENIX_PACK_SHARED_PARTICLE, SPHENIX_PACK_FORCE_PARTICLE,
   * or SPHENIX_PACK_GRADIENT_PARTICLE */
#pragma error "how did we get here...?"
#endif

#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* ---------- */
  /* SoA access */
  /* ---------- */

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const double* x = part_get_const_x_explicit(pd, i);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].x_h.w = part_get_h_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const float* v = part_get_const_v_explicit(pd, i);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].vx_m.w = part_get_mass_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u_explicit(pd, i);
#else
    ps[i].u_rho_f_p.x = part_get_u_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.y = part_get_rho_explicit(pd, i);
#else
    ps[i].u_rho_f_p.y = part_get_rho_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.z = part_get_soundspeed_explicit(pd, i);
#else
    ps[i].u_rho_f_p.z = part_get_f_gradh_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.w = part_get_alpha_av_explicit(pd, i);
#else
    ps[i].u_rho_f_p.w = part_get_pressure_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.x = part_get_f_gradh_explicit(pd, i);
#else
    ps[i].bals_c_avisc_adiff.x = part_get_balsara_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.y = part_get_pressure_explicit(pd, i);
#else
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.z = part_get_balsara_explicit(pd, i);
#else
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff_explicit(pd, i);
#else
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff_explicit(pd, i);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin_explicit(pd, i);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin_explicit(pd, i);
    ps[i].timebin_minngbtimebin_pjs_pje.y = mintbin;
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    ps[i].timebin_minngbtimebin_pjs_pje.z = cjstart;
    ps[i].timebin_minngbtimebin_pjs_pje.w = cjend;
  }
#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
#endif
}
