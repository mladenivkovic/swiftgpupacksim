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
#ifndef CUDA_GPU_PART_STRUCTS_H
#define CUDA_GPU_PART_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WITH_CUDA
#include <vector_types.h>
#endif

/* Config parameters. */
#include "../../config.h"
#include "../align.h"


/*! Container for particle data required for density calcs */
struct gpu_part_send_d {
#ifdef WITH_CUDA

  /*! Particle position and h -> x, y, z, h */
  float4 x_h;

  /*! Particle predicted velocity and mass -> vx, vy, vz, m */
  float4 vx_m;

  /*! Start and end index of particles to be interacted with in particle
   * buffer arrays */
  int2 pjs_pje;

#endif
} SWIFT_STRUCT_ALIGN;

/*! Container for particle data sent back to CPU for density calcs */
struct gpu_part_recv_d {
#ifdef WITH_CUDA

  /*! rho, rho_dh, wcount, wcount_dh */
  float4 rho_rhodh_wcount_wcount_dh;

  /*! Particle velocity curl; rot_ux and velocity divergence; div_v */
  float4 rot_vx_div_v;

#endif
} SWIFT_STRUCT_ALIGN;


/*! Container for particle data required for gradient calcs */
struct gpu_part_send_g {
#ifdef WITH_CUDA

  /*! Particle position & smoothing length */
  float4 x_h;

  /*! Particle velocity and mass */
  float4 vx_m;

#if defined(SPHENIX_PACK_FORCE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE) || \
    defined(SPHENIX_PACK_FORCE_TYPE_PARTICLE)

  /*! Internal energy, density, particle soundspeed, alpha constant for
   * viscosity */
  float4 u_rho_c_avisc;

  /*! Max viscosity alpha among neighbours, signal velocity, laplacian of
   * intenal energy */
  float3 aviscmax_vsig_lapu;

#else

  /*! internal energy, particle density, and speed of sound c, max alpha
   * viscosity among neighbours */
  float4 u_rho_c_aviscmax;

  /*! viscosity information results */
  float3 avisc_vsig_lapu;

#endif

  /*! Start and end index of particles to be interacted with in particle
   * buffer arrays */
  int2 pjs_pje;

#endif
} SWIFT_STRUCT_GPU_PART_SEND_G_ALIGN;

/*! Container for particle data sent back to CPU for gradient calcs */
struct gpu_part_recv_g {
#ifdef WITH_CUDA

  /*! viscosity information results */
  float3 aviscmax_vsig_lapu;

#endif
} SWIFT_STRUCT_GPU_PART_RECV_G_ALIGN;

/*! Container for particle data required for force calcs */
struct gpu_part_send_f {
#ifdef WITH_CUDA

  /* Data required for the calculation: Values read to local GPU memory */

  /*! Particle positions, smoothing length */
  float4 x_h;

  /*! Particle predicted velocity and mass */
  float4 vx_m;

#if defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
    defined(SPHENIX_PACK_GRADIENT_TYPE_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_PARTICLE) || \
    defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE)

  /*! Internal energy, density, particle soundspeed, alpha constant for
   * viscosity */
  float4 u_rho_c_avisc;

  /*! Variable smoothing length term f, pressure, balsara, alpha constant for
   * diffusion */
  float4 f_p_balsara_adiff;

#else

  /*! Internal energy, density, variable smoothing length term f, pressure */
  float4 u_rho_f_p;

  /*! balsara, particle speed of sound, viscosity parameter alpha, diffusion
   * parameter alpha */
  float4 bals_c_avisc_adiff;

#endif

  /*! Particle timebin, initial value of min neighbour timebin, start
   * and end index of particles to be interacted with in particle buffer
   * arrays */
  int4 timebin_minngbtimebin_pjs_pje;

#endif
} SWIFT_STRUCT_GPU_PART_SEND_F_ALIGN;

/*! Container for particle data sent back to CPU for force calcs */
struct gpu_part_recv_f {
#ifdef WITH_CUDA

  /*! Particle acceleration vector */
  float3 a_hydro;

  /*! change of u and h with dt, v_sig */
  float3 udt_hdt_minngbtb;

#endif
} SWIFT_STRUCT_GPU_PART_RECV_F_ALIGN;

#ifdef __cplusplus
}
#endif

#endif  // CUDA_GPU_PART_STRUCTS_H
