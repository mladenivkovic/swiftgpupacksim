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
  struct part* restrict cp = cell_get_hydro_parts(c);

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

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho = part_get_rho(p) + pr.rho_rhodh_wcount_wcount_dh.x;
    part_set_rho(p, rho);

    float rho_dh = part_get_rho_dh(p) + pr.rho_rhodh_wcount_wcount_dh.y;
    part_set_rho_dh(p, rho_dh);

    float wcount = part_get_wcount(p) + pr.rho_rhodh_wcount_wcount_dh.z;
    part_set_wcount(p, wcount);

    float wcount_dh = part_get_wcount_dh(p) + pr.rho_rhodh_wcount_wcount_dh.w;
    part_set_wcount_dh(p, wcount_dh);

    float* rot_v = part_get_rot_v(p);
    rot_v[0] += pr.rot_vx_div_v.x;
    rot_v[1] += pr.rot_vx_div_v.y;
    rot_v[2] += pr.rot_vx_div_v.z;

    float div_v = part_get_div_v(p) + pr.rot_vx_div_v.w;
    part_set_div_v(p, div_v);
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

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho = part_get_rho(p) + pr.rho_rhodh_wcount_wcount_dh.x;
    part_set_rho(p, rho);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct density_unpack */

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_d pr = parts_recv[i];

    float rho_dh = part_get_rho_dh(p) + pr.rho_rhodh_wcount_wcount_dh.y;
    part_set_rho_dh(p, rho_dh);

    float wcount = part_get_wcount(p) + pr.rho_rhodh_wcount_wcount_dh.z;
    part_set_wcount(p, wcount);

    float wcount_dh = part_get_wcount_dh(p) + pr.rho_rhodh_wcount_wcount_dh.w;
    part_set_wcount_dh(p, wcount_dh);

    float* rot_v = part_get_rot_v(p);
    rot_v[0] += pr.rot_vx_div_v.x;
    rot_v[1] += pr.rot_vx_div_v.y;
    rot_v[2] += pr.rot_vx_div_v.z;

    float div_v = part_get_div_v(p) + pr.rot_vx_div_v.w;
    part_set_div_v(p, div_v);
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
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float rho = part_get_rho(p) + pr.rho_rhodh_wcount_wcount_dh.x;
    part_set_rho(p, rho);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];
    float rho_dh = part_get_rho_dh(p) + pr.rho_rhodh_wcount_wcount_dh.y;
    part_set_rho_dh(p, rho_dh);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float wcount = part_get_wcount(p) + pr.rho_rhodh_wcount_wcount_dh.z;
    part_set_wcount(p, wcount);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float wcount_dh = part_get_wcount_dh(p) + pr.rho_rhodh_wcount_wcount_dh.w;
    part_set_wcount_dh(p, wcount_dh);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float* rot_v = part_get_rot_v(p);
    rot_v[0] += pr.rot_vx_div_v.x;
    rot_v[1] += pr.rot_vx_div_v.y;
    rot_v[2] += pr.rot_vx_div_v.z;
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_d pr = parts_recv[i];

    float div_v = part_get_div_v(p) + pr.rot_vx_div_v.w;
    part_set_div_v(p, div_v);
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
  struct part* restrict cp = cell_get_hydro_parts(c);

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

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_g pr = parts_recv[i];

    float avisc_old = part_get_alpha_visc_max_ngb(p);
    float avisc = fmaxf(avisc_old, pr.aviscmax_vsig_lapu.x);
    part_set_alpha_visc_max_ngb(p, avisc);

    float vsig = fmaxf(pr.aviscmax_vsig_lapu.y, part_get_v_sig(p));
    part_set_v_sig(p, vsig);

    float lu = pr.aviscmax_vsig_lapu.z + part_get_laplace_u(p);
    part_set_laplace_u(p, lu);
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

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_g pr = parts_recv[i];

    float avisc_old = part_get_alpha_visc_max_ngb(p);
    float avisc = fmaxf(avisc_old, pr.aviscmax_vsig_lapu.x);
    part_set_alpha_visc_max_ngb(p, avisc);

    float vsig = fmaxf(pr.aviscmax_vsig_lapu.y, part_get_v_sig(p));
    part_set_v_sig(p, vsig);

    float lu = pr.aviscmax_vsig_lapu.z + part_get_laplace_u(p);
    part_set_laplace_u(p, lu);
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
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_g pr = parts_recv[i];

    float avisc_old = part_get_alpha_visc_max_ngb(p);
    float avisc = fmaxf(avisc_old, pr.aviscmax_vsig_lapu.x);
    part_set_alpha_visc_max_ngb(p, avisc);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_g pr = parts_recv[i];

    float vsig = fmaxf(pr.aviscmax_vsig_lapu.y, part_get_v_sig(p));
    part_set_v_sig(p, vsig);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_g pr = parts_recv[i];

    float lu = pr.aviscmax_vsig_lapu.z + part_get_laplace_u(p);
    part_set_laplace_u(p, lu);
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
  struct part* restrict cp = cell_get_hydro_parts(c);

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

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    float* a = part_get_a_hydro(p);
    a[0] += pr.a_hydro.x;
    a[1] += pr.a_hydro.y;
    a[2] += pr.a_hydro.z;

    float u_dt = pr.udt_hdt_minngbtb.x + part_get_u_dt(p);
    part_set_u_dt(p, u_dt);

    float h_dt = pr.udt_hdt_minngbtb.y + part_get_h_dt(p);
    part_set_h_dt(p, h_dt);

    timebin_t mintbin = (timebin_t)(pr.udt_hdt_minngbtb.z + 0.5f);
    part_set_timestep_limiter_min_ngb_time_bin(p, mintbin);
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

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    float* a = part_get_a_hydro(p);
    a[0] += pr.a_hydro.x;
    a[1] += pr.a_hydro.y;
    a[2] += pr.a_hydro.z;

    float u_dt = pr.udt_hdt_minngbtb.x + part_get_u_dt(p);
    part_set_u_dt(p, u_dt);

    float h_dt = pr.udt_hdt_minngbtb.y + part_get_h_dt(p);
    part_set_h_dt(p, h_dt);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */

    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;

    struct gpu_part_recv_f pr = parts_recv[i];

    timebin_t mintbin = (timebin_t)(pr.udt_hdt_minngbtb.z + 0.5f);
    part_set_timestep_limiter_min_ngb_time_bin(p, mintbin);
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
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];

    float* a = part_get_a_hydro(p);
    a[0] += pr.a_hydro.x;
    a[1] += pr.a_hydro.y;
    a[2] += pr.a_hydro.z;
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];

    float u_dt = pr.udt_hdt_minngbtb.x + part_get_u_dt(p);
    part_set_u_dt(p, u_dt);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];
    float h_dt = pr.udt_hdt_minngbtb.y + part_get_h_dt(p);

    part_set_h_dt(p, h_dt);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    struct part* restrict p = &cp[i];
    if (!part_is_active(p, e)) continue;
    struct gpu_part_recv_f pr = parts_recv[i];

    timebin_t mintbin = (timebin_t)(pr.udt_hdt_minngbtb.z + 0.5f);
    part_set_timestep_limiter_min_ngb_time_bin(p, mintbin);
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
  const struct part* restrict parts = cell_get_const_hydro_parts(c);
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

    const struct part* restrict p = &parts[i];

    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h(p);

    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass(p);

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

    const struct part* restrict p = &parts[i];

    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h(p);

    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass(p);

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
    const struct part* restrict p = &parts[i];

    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    ps[i].x_h.w = part_get_h(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];

    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    ps[i].vx_m.w = part_get_mass(p);
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
  const struct part* restrict parts = cell_get_const_hydro_parts(ci);
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

    const struct part* restrict p = &parts[i];

    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h(p);

    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass(p);

#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u(p);
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av(p);

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb(p);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig(p);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u(p);
#else
    ps[i].u_rho_c_aviscmax.x = part_get_u(p);
    ps[i].u_rho_c_aviscmax.y = part_get_rho(p);
    ps[i].u_rho_c_aviscmax.z = part_get_soundspeed(p);
    ps[i].u_rho_c_aviscmax.w = part_get_alpha_visc_max_ngb(p);

    ps[i].avisc_vsig_lapu.x = part_get_alpha_av(p);
    ps[i].avisc_vsig_lapu.y = part_get_v_sig(p);
    ps[i].avisc_vsig_lapu.z = part_get_laplace_u(p);
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

    const struct part* restrict p = &parts[i];

    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h(p);

    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass(p);
  }

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE)
#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    const struct part* restrict p = &parts[i];

    ps[i].u_rho_c_aviscmax.x = part_get_u(p);
    ps[i].u_rho_c_aviscmax.y = part_get_rho(p);
    ps[i].u_rho_c_aviscmax.z = part_get_soundspeed(p);
    ps[i].u_rho_c_aviscmax.w = part_get_alpha_visc_max_ngb(p);

    ps[i].avisc_vsig_lapu.x = part_get_alpha_av(p);
    ps[i].avisc_vsig_lapu.y = part_get_v_sig(p);
    ps[i].avisc_vsig_lapu.z = part_get_laplace_u(p);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SPHENIX_PACK_FORCE_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_gradient_share_pack */

    const struct part* restrict p = &parts[i];

    ps[i].u_rho_c_avisc.x = part_get_u(p);
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    const struct part* restrict p = &parts[i];

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb(p);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig(p);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u(p);

    ps[i].pjs_pje.x = cjstart;
    ps[i].pjs_pje.y = cjend;
  }

#elif defined(SPHENIX_PACK_SHARED_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_gradient_share_pack */

    const struct part* restrict p = &parts[i];

    ps[i].u_rho_c_avisc.x = part_get_u(p);
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */
    const struct part* restrict p = &parts[i];
    ps[i].u_rho_c_avisc.x = part_get_alpha_av(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    const struct part* restrict p = &parts[i];

    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb(p);
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig(p);
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u(p);

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
    const struct part* restrict p = &parts[i];
    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    ps[i].x_h.w = part_get_h(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    ps[i].vx_m.w = part_get_mass(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u(p);
#else
    ps[i].u_rho_c_aviscmax.x = part_get_u(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
#else
    ps[i].u_rho_c_aviscmax.y = part_get_rho(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
#else
    ps[i].u_rho_c_aviscmax.z = part_get_soundspeed(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.w = part_get_alpha_av(p);
#else
    ps[i].u_rho_c_aviscmax.w = part_get_alpha_visc_max_ngb(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].aviscmax_vsig_lapu.x = part_get_alpha_visc_max_ngb(p);
#else
    ps[i].avisc_vsig_lapu.x = part_get_alpha_av(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].aviscmax_vsig_lapu.y = part_get_v_sig(p);
#else
    ps[i].avisc_vsig_lapu.y = part_get_v_sig(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].aviscmax_vsig_lapu.z = part_get_laplace_u(p);
#else
    ps[i].avisc_vsig_lapu.z = part_get_laplace_u(p);
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
  const struct part* restrict parts = cell_get_const_hydro_parts(ci);
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

    const struct part* restrict p = &parts[i];

    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h(p);

    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass(p);

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u(p);
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av(p);

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh(p);
    ps[i].f_p_balsara_adiff.y = part_get_pressure(p);
    ps[i].f_p_balsara_adiff.z = part_get_balsara(p);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff(p);
#else
    ps[i].u_rho_f_p.x = part_get_u(p);
    ps[i].u_rho_f_p.y = part_get_rho(p);
    ps[i].u_rho_f_p.z = part_get_f_gradh(p);
    ps[i].u_rho_f_p.w = part_get_pressure(p);

    ps[i].bals_c_avisc_adiff.x = part_get_balsara(p);
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed(p);
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av(p);
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff(p);
#endif

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin(p);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin(p);
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

    const struct part* restrict p = &parts[i];

    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
    ps[i].x_h.w = part_get_h(p);

    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
    ps[i].vx_m.w = part_get_mass(p);
  }

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE)

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct gradient_pack */

    const struct part* restrict p = &parts[i];

    ps[i].u_rho_c_avisc.x = part_get_u(p);
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
    ps[i].u_rho_c_avisc.w = part_get_alpha_av(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */

    const struct part* restrict p = &parts[i];

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh(p);
    ps[i].f_p_balsara_adiff.y = part_get_pressure(p);
    ps[i].f_p_balsara_adiff.z = part_get_balsara(p);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff(p);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin(p);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin(p);
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

    const struct part* restrict p = &parts[i];

    ps[i].u_rho_f_p.x = part_get_u(p);
    ps[i].u_rho_f_p.y = part_get_rho(p);
    ps[i].u_rho_f_p.z = part_get_f_gradh(p);
    ps[i].u_rho_f_p.w = part_get_pressure(p);

    ps[i].bals_c_avisc_adiff.x = part_get_balsara(p);
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed(p);
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av(p);
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff(p);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin(p);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin(p);
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

    const struct part* restrict p = &parts[i];

    ps[i].u_rho_c_avisc.x = part_get_u(p);
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
    ps[i].u_rho_c_avisc.z = part_get_alpha_av(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    /* struct force_pack */

    const struct part* restrict p = &parts[i];

    ps[i].f_p_balsara_adiff.x = part_get_f_gradh(p);
    ps[i].f_p_balsara_adiff.y = part_get_pressure(p);
    ps[i].f_p_balsara_adiff.z = part_get_balsara(p);
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff(p);

    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin(p);
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin(p);
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
    const struct part* restrict p = &parts[i];
    const double* x = part_get_const_x(p);
    ps[i].x_h.x = x[0] - shift[0];
    ps[i].x_h.y = x[1] - shift[1];
    ps[i].x_h.z = x[2] - shift[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    ps[i].x_h.w = part_get_h(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    const float* v = part_get_const_v(p);
    ps[i].vx_m.x = v[0];
    ps[i].vx_m.y = v[1];
    ps[i].vx_m.z = v[2];
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    ps[i].vx_m.w = part_get_mass(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.x = part_get_u(p);
#else
    ps[i].u_rho_f_p.x = part_get_u(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.y = part_get_rho(p);
#else
    ps[i].u_rho_f_p.y = part_get_rho(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.z = part_get_soundspeed(p);
#else
    ps[i].u_rho_f_p.z = part_get_f_gradh(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].u_rho_c_avisc.w = part_get_alpha_av(p);
#else
    ps[i].u_rho_f_p.w = part_get_pressure(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.x = part_get_f_gradh(p);
#else
    ps[i].bals_c_avisc_adiff.x = part_get_balsara(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.y = part_get_pressure(p);
#else
    ps[i].bals_c_avisc_adiff.y = part_get_soundspeed(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.z = part_get_balsara(p);
#else
    ps[i].bals_c_avisc_adiff.z = part_get_alpha_av(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE)
    ps[i].f_p_balsara_adiff.w = part_get_alpha_diff(p);
#else
    ps[i].bals_c_avisc_adiff.w = part_get_alpha_diff(p);
#endif
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    ps[i].timebin_minngbtimebin_pjs_pje.x = (int)part_get_time_bin(p);
  }

#ifdef VECTORIZE
#pragma omp simd
#endif
  for (int i = 0; i < count; i++) {
    const struct part* restrict p = &parts[i];
    int mintbin = (int)part_get_timestep_limiter_min_ngb_time_bin(p);
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
