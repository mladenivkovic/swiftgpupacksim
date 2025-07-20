/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2019 Josh Borrow (joshua.borrow@durham.ac.uk) &
 *                    Matthieu Schaller (schaller@strw.leidenuniv.nl)
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
#ifndef SWIFT_SPHENIX_HYDRO_PART_H
#define SWIFT_SPHENIX_HYDRO_PART_H

/**
 * @file SPHENIX/hydro_part.h
 * @brief Density-Energy conservative implementation of SPH,
 *        with added SPHENIX physics (Borrow 2020) (particle definition)
 */

#include "align.h"
#include "error.h"
#include "hydro_part_placeholder_structs.h"
#include "timestep_limiter_struct.h"

#include <stddef.h>


/**
 * @brief Particle fields for the SPH particles
 *
 * The density and force substructures are used to contain variables only used
 * within the density and force loops over neighbours. All more permanent
 * variables should be declared in the main part of the part structure,
 */
struct part {

  /*! Particle unique ID. */
  long long _id;

  /*! Pointer to corresponding gravity part. */
  struct gpart *_gpart;

  /*! Particle position. */
  double _x[3];

  /*! Particle predicted velocity. */
  float _v[3];

  /*! Particle acceleration. */
  float _a_hydro[3];

  /*! Particle mass. */
  float _mass;

  /*! Particle smoothing length. */
  float _h;

  /*! Particle internal energy. */
  float _u;

  /*! Time derivative of the internal energy. */
  float _u_dt;

  /*! Particle density. */
  float _rho;

  /* Store viscosity information in a separate struct. */
  struct {

    /*! Particle velocity divergence */
    float _div_v;

    /*! Time differential of velocity divergence */
    float _div_v_dt;

    /*! Particle velocity divergence from previous step */
    float _div_v_previous_step;

    /*! Artificial viscosity parameter */
    float _alpha;

    /*! Signal velocity */
    float _v_sig;

  } viscosity;

  /* Store thermal diffusion information in a separate struct. */
  struct {

    /*! del^2 u, a smoothed quantity */
    float _laplace_u;

    /*! Thermal diffusion coefficient */
    float _alpha;

  } diffusion;

  /* Store density/force specific stuff. */

  union {
    /**
     * @brief Structure for the variables only used in the density loop over
     * neighbours.
     *
     * Quantities in this sub-structure should only be accessed in the density
     * loop over neighbours and the ghost task.
     */
    struct {

      /*! Neighbour number count. */
      float _wcount;

      /*! Derivative of the neighbour number with respect to h. */
      float _wcount_dh;

      /*! Derivative of density with respect to h */
      float _rho_dh;

      /*! Particle velocity curl. */
      float _rot_v[3];

    } density;

    /**
     * @brief Structure for the variables only used in the force loop over
     * neighbours.
     *
     * Quantities in this sub-structure should only be accessed in the force
     * loop over neighbours and the ghost, drift and kick tasks.
     */
    struct {

      /*! "Grad h" term -- only partial in P-U */
      float _f_gradh;

      /*! Particle pressure. */
      float _pressure;

      /*! Particle soundspeed. */
      float _soundspeed;

      /*! Time derivative of smoothing length  */
      float _h_dt;

      /*! Balsara switch */
      float _balsara;

      /*! Maximal alpha (viscosity) over neighbours */
      float _alpha_visc_max_ngb;

    } force;
  };

  /*! Additional data used for adaptive softening */
  struct adaptive_softening_part_data _adaptive_softening_data;

  /*! Additional data used by the MHD scheme */
  struct mhd_part_data _mhd_data;

  /*! Chemistry information */
  struct chemistry_part_data _chemistry_data;

  /*! Cooling information */
  struct cooling_part_data _cooling_data;

  /*! Additional data used by the feedback */
  struct feedback_part_data _feedback_data;

  /*! Black holes information (e.g. swallowing ID) */
  struct black_holes_part_data _black_holes_data;

  /*! Sink information (e.g. swallowing ID) */
  struct sink_part_data _sink_data;

  /*! Additional data used by the pressure floor */
  struct pressure_floor_part_data _pressure_floor_data;

  /*! Additional Radiative Transfer Data */
  struct rt_part_data _rt_data;

  /*! RT sub-cycling time stepping data */
  struct rt_timestepping_data _rt_time_data;

  /*! Tree-depth at which size / 2 <= h * gamma < size */
  char _depth_h;

  /*! Time-step length */
  timebin_t _time_bin;

  /*! Time-step limiter information */
  struct timestep_limiter_data _limiter_data;

#ifdef SWIFT_DEBUG_CHECKS

  /* Time of the last drift */
  integertime_t _ti_drift;

  /* Time of the last kick */
  integertime_t _ti_kick;

#endif

#ifdef SWIFT_HYDRO_DENSITY_CHECKS

  /* Integer number of neighbours in the density loop */
  int N_density;

  /* Exact integer number of neighbours in the density loop */
  int N_density_exact;

  /* Integer number of neighbours in the gradient loop */
  int N_gradient;

  /* Exact integer number of neighbours in the gradient loop */
  int N_gradient_exact;

  /* Integer number of neighbours in the force loop */
  int N_force;

  /* Exact integer number of neighbours in the force loop */
  int N_force_exact;

  /*! Exact value of the density field obtained via brute-force loop */
  float rho_exact;

  /*! Weighted number of neighbours in the density loop */
  float n_density;

  /*! Exact value of the weighted number of neighbours in the density loop */
  float n_density_exact;

  /*! Weighted number of neighbours in the gradient loop */
  float n_gradient;

  /*! Exact value of the weighted number of neighbours in the gradient loop */
  float n_gradient_exact;

  /*! Weighted number of neighbours in the force loop */
  float n_force;

  /*! Exact value of the weighted number of neighbours in the force loop */
  float n_force_exact;

  /*! Has this particle interacted with any unhibited neighbour? */
  char inhibited_exact;

  /*! Has this particle been woken up by the limiter? */
  char limited_part;
#endif

  /*! Geometrical quantities used for Finite Volume Particle Method RT. */
  struct fvpm_geometry_struct _geometry;

} SWIFT_STRUCT_ALIGN;

static __attribute__((always_inline)) INLINE long long part_get_id(
    const struct part *restrict p) {
  return p->_id;
}

static __attribute__((always_inline)) INLINE long long *part_get_id_p(
    struct part *restrict p) {
  return &p->_id;
}

static __attribute__((always_inline)) INLINE const long long *
part_get_const_id_p(const struct part *restrict p) {
  return &p->_id;
}

static __attribute__((always_inline)) INLINE void part_set_id(
    struct part *restrict p, const long long id) {
  p->_id = id;
}

static __attribute__((always_inline)) INLINE struct gpart *part_get_gpart(
    const struct part *restrict p) {
  return p->_gpart;
}

static __attribute__((always_inline)) INLINE struct gpart **part_get_gpart_p(
    struct part *restrict p) {
  return &p->_gpart;
}

static __attribute__((always_inline)) INLINE const struct gpart *
part_get_const_gpart(const struct part *restrict p) {
  return p->_gpart;
}

static __attribute__((always_inline)) INLINE void part_set_gpart(
    struct part *restrict p, struct gpart *gp) {
  p->_gpart = gp;
}

/**
 * @brief get particle position
 */
static __attribute__((always_inline)) INLINE double *part_get_x(
    struct part *restrict p) {
  return p->_x;
}

/**
 * @brief get particle position for read-only access
 */
static __attribute__((always_inline)) INLINE const double *part_get_const_x(
    const struct part *restrict p) {
  return p->_x;
}

/**
 * @brief get particle position
 */
static __attribute__((always_inline)) INLINE double part_get_x_ind(
    const struct part *restrict p, const size_t ind) {
  return p->_x[ind];
}

/**
 * @brief set all x-values of part p from an array.
 */
static __attribute__((always_inline)) INLINE void part_set_x(
    struct part *restrict p, const double x[3]) {
  p->_x[0] = x[0];
  p->_x[1] = x[1];
  p->_x[2] = x[2];
}

/**
 * @brief set x-value by index.
 */
static __attribute__((always_inline)) INLINE void part_set_x_ind(
    struct part *restrict p, const size_t i, const double x) {
  p->_x[i] = x;
}

/**
 * @brief get particle velocity
 * Note: Avoid when you have access to const particle pointer. Use
 * access by index instead via part_get_v_ind()
 */
static __attribute__((always_inline)) INLINE float *part_get_v(
    struct part *restrict p) {
  return p->_v;
}

/**
 * @brief get particle velocity for read-only access.
 * Note: Avoid when you have access to const particle pointer. Use
 * access by index instead via part_get_v_ind()
 */
static __attribute__((always_inline)) INLINE const float *part_get_const_v(
    const struct part *restrict p) {
  return p->_v;
}

/**
 * @brief get particle velocity by index
 */
static __attribute__((always_inline)) INLINE float part_get_v_ind(
    const struct part *restrict p, const size_t ind) {
  return p->_v[ind];
}

/**
 * @brief set all v-values of part p from an array.
 */
static __attribute__((always_inline)) INLINE void part_set_v(
    struct part *restrict p, const float v[3]) {
  p->_v[0] = v[0];
  p->_v[1] = v[1];
  p->_v[2] = v[2];
}

/**
 * @brief set v-value by index.
 */
static __attribute__((always_inline)) INLINE void part_set_v_ind(
    struct part *restrict p, const size_t i, const float v) {
  p->_v[i] = v;
}

/**
 * @brief get particle hydrodynamical acceleration. This is for read-write
 * access. Avoid when you have access to const particle pointer. Use
 * access by index instead via part_get_a_hydro_ind()
 */
static __attribute__((always_inline)) INLINE float *part_get_a_hydro(
    struct part *restrict p) {
  return p->_a_hydro;
}

/**
 * @brief get particle hydrodynamical acceleration.
 * Note: Avoid when you have access to const particle pointer. Use
 * access by index instead via part_get_a_hydro_ind()
 */
static __attribute__((always_inline)) INLINE const float *
part_get_const_a_hydro(const struct part *restrict p) {
  return p->_a_hydro;
}

/**
 * @brief get particle hydrodynamical acceleration by index.
 */
static __attribute__((always_inline)) INLINE float part_get_a_hydro_ind(
    const struct part *restrict p, size_t ind) {
  return p->_a_hydro[ind];
}

/**
 * @brief set all a_hydro-values of part p from an array.
 */
static __attribute__((always_inline)) INLINE void part_set_a_hydro(
    struct part *restrict p, const float a_hydro[3]) {
  p->_a_hydro[0] = a_hydro[0];
  p->_a_hydro[1] = a_hydro[1];
  p->_a_hydro[2] = a_hydro[2];
}

/**
 * @brief set a_hydro-value by index.
 */
static __attribute__((always_inline)) INLINE void part_set_a_hydro_ind(
    struct part *restrict p, const size_t i, const float a_hydro) {
  p->_a_hydro[i] = a_hydro;
}

static __attribute__((always_inline)) INLINE float part_get_mass(
    const struct part *restrict p) {
  return p->_mass;
}

static __attribute__((always_inline)) INLINE float *part_get_mass_p(
    struct part *restrict p) {
  return &p->_mass;
}
static __attribute__((always_inline)) INLINE const float *part_get_const_mass_p(
    const struct part *restrict p) {
  return &p->_mass;
}

static __attribute__((always_inline)) INLINE void part_set_mass(
    struct part *restrict p, const float mass) {
  p->_mass = mass;
}

/**
 * @brief get smoothing length
 */
static __attribute__((always_inline)) INLINE float part_get_h(
    const struct part *restrict p) {
  return p->_h;
}

static __attribute__((always_inline)) INLINE float *part_get_h_p(
    struct part *restrict p) {
  return &p->_h;
}

static __attribute__((always_inline)) INLINE const float *part_get_const_h_p(
    const struct part *restrict p) {
  return &p->_h;
}

static __attribute__((always_inline)) INLINE void part_set_h(
    struct part *restrict p, const float h) {
  p->_h = h;
}

static __attribute__((always_inline)) INLINE float part_get_u(
    const struct part *restrict p) {
  return p->_u;
}

static __attribute__((always_inline)) INLINE float *part_get_u_p(
    struct part *restrict p) {
  return &p->_u;
}

static __attribute__((always_inline)) INLINE const float *part_get_const_u_p(
    const struct part *restrict p) {
  return &p->_u;
}

static __attribute__((always_inline)) INLINE void part_set_u(
    struct part *restrict p, const float u) {
  p->_u = u;
}

static __attribute__((always_inline)) INLINE float part_get_rho(
    const struct part *restrict p) {
  return p->_rho;
}

static __attribute__((always_inline)) INLINE float *part_get_rho_p(
    struct part *restrict p) {
  return &p->_rho;
}

static __attribute__((always_inline)) INLINE const float *part_get_const_rho_p(
    const struct part *restrict p) {
  return &p->_rho;
}

static __attribute__((always_inline)) INLINE void part_set_rho(
    struct part *restrict p, const float rho) {
  p->_rho = rho;
}

/**
 * @brief get du/dt
 */
static __attribute__((always_inline)) INLINE float part_get_u_dt(
    const struct part *restrict p) {
  return p->_u_dt;
}

static __attribute__((always_inline)) INLINE float *part_get_u_dt_p(
    struct part *restrict p) {
  return &p->_u_dt;
}

static __attribute__((always_inline)) INLINE void part_set_u_dt(
    struct part *restrict p, const float u_dt) {
  p->_u_dt = u_dt;
}

static __attribute__((always_inline)) INLINE float part_get_div_v(
    const struct part *restrict p) {
  return p->viscosity._div_v;
}

static __attribute__((always_inline)) INLINE float *part_get_div_v_p(
    struct part *restrict p) {
  return &p->viscosity._div_v;
}

static __attribute__((always_inline)) INLINE const float *
part_get_const_div_v_p(const struct part *restrict p) {
  return &p->viscosity._div_v;
}

static __attribute__((always_inline)) INLINE void part_set_div_v(
    struct part *restrict p, const float div_v) {
  p->viscosity._div_v = div_v;
}

static __attribute__((always_inline)) INLINE float part_get_div_v_dt(
    const struct part *restrict p) {
  return p->viscosity._div_v_dt;
}

static __attribute__((always_inline)) INLINE float *part_get_div_v_dt_p(
    struct part *restrict p) {
  return &p->viscosity._div_v_dt;
}

static __attribute__((always_inline)) INLINE const float *
part_get_const_div_v_dt_p(const struct part *restrict p) {
  return &p->viscosity._div_v_dt;
}

static __attribute__((always_inline)) INLINE void part_set_div_v_dt(
    struct part *restrict p, const float div_v_dt) {
  p->viscosity._div_v_dt = div_v_dt;
}

static __attribute__((always_inline)) INLINE float part_get_div_v_previous_step(
    const struct part *restrict p) {
  return p->viscosity._div_v_previous_step;
}

static __attribute__((always_inline)) INLINE float *
part_get_div_v_previous_step_p(struct part *restrict p) {
  return &p->viscosity._div_v_previous_step;
}

static __attribute__((always_inline)) INLINE void part_set_div_v_previous_step(
    struct part *restrict p, const float div_v_previous_step) {
  p->viscosity._div_v_previous_step = div_v_previous_step;
}

/**
 * @brief get the artificial viscosity parameter alpha
 */
static __attribute__((always_inline)) INLINE float part_get_alpha_av(
    const struct part *restrict p) {
  return p->viscosity._alpha;
}

static __attribute__((always_inline)) INLINE float *part_get_alpha_av_p(
    struct part *restrict p) {
  return &p->viscosity._alpha;
}

/**
 * @brief set the artificial viscosity parameter alpha
 */
static __attribute__((always_inline)) INLINE void part_set_alpha_av(
    struct part *restrict p, const float alpha) {
  p->viscosity._alpha = alpha;
}

static __attribute__((always_inline)) INLINE float part_get_v_sig(
    const struct part *restrict p) {
  return p->viscosity._v_sig;
}

static __attribute__((always_inline)) INLINE float *part_get_v_sig_p(
    struct part *restrict p) {
  return &p->viscosity._v_sig;
}

static __attribute__((always_inline)) INLINE void part_set_v_sig(
    struct part *restrict p, const float v_sig) {
  p->viscosity._v_sig = v_sig;
}

static __attribute__((always_inline)) INLINE float part_get_laplace_u(
    const struct part *restrict p) {
  return p->diffusion._laplace_u;
}

static __attribute__((always_inline)) INLINE float *part_get_laplace_u_p(
    struct part *restrict p) {
  return &p->diffusion._laplace_u;
}

static __attribute__((always_inline)) INLINE const float *
part_get_const_laplace_u_p(const struct part *restrict p) {
  return &p->diffusion._laplace_u;
}

static __attribute__((always_inline)) INLINE void part_set_laplace_u(
    struct part *restrict p, const float laplace_u) {
  p->diffusion._laplace_u = laplace_u;
}

/**
 * @brief get the thermal diffusion coefficient alpha
 */
static __attribute__((always_inline)) INLINE float part_get_alpha_diff(
    const struct part *restrict p) {
  return p->diffusion._alpha;
}

static __attribute__((always_inline)) INLINE float *part_get_alpha_diff_p(
    struct part *restrict p) {
  return &p->diffusion._alpha;
}

/**
 * @brief set the thermal diffusion coefficient alpha
 */
static __attribute__((always_inline)) INLINE void part_set_alpha_diff(
    struct part *restrict p, const float alpha) {
  p->diffusion._alpha = alpha;
}

static __attribute__((always_inline)) INLINE float part_get_wcount(
    const struct part *restrict p) {
  return p->density._wcount;
}

static __attribute__((always_inline)) INLINE float *part_get_wcount_p(
    struct part *restrict p) {
  return &p->density._wcount;
}

static __attribute__((always_inline)) INLINE void part_set_wcount(
    struct part *restrict p, const float wcount) {
  p->density._wcount = wcount;
}

static __attribute__((always_inline)) INLINE float part_get_wcount_dh(
    const struct part *restrict p) {
  return p->density._wcount_dh;
}

static __attribute__((always_inline)) INLINE float *part_get_wcount_dh_p(
    struct part *restrict p) {
  return &p->density._wcount_dh;
}

static __attribute__((always_inline)) INLINE void part_set_wcount_dh(
    struct part *restrict p, const float wcount_dh) {
  p->density._wcount_dh = wcount_dh;
}

static __attribute__((always_inline)) INLINE float part_get_rho_dh(
    const struct part *restrict p) {
  return p->density._rho_dh;
}

static __attribute__((always_inline)) INLINE float *part_get_rho_dh_p(
    struct part *restrict p) {
  return &p->density._rho_dh;
}

static __attribute__((always_inline)) INLINE void part_set_rho_dh(
    struct part *restrict p, const float rho_dh) {
  p->density._rho_dh = rho_dh;
}

/**
 * @brief get particle velocity curl
 */
static __attribute__((always_inline)) INLINE float *part_get_rot_v(
    struct part *restrict p) {
  return p->density._rot_v;
}

static __attribute__((always_inline)) INLINE float part_get_rot_v_ind(
    const struct part *restrict p, size_t ind) {
  return p->density._rot_v[ind];
}

/**
 * @brief set all rot_v-values of part p from an array.
 */
static __attribute__((always_inline)) INLINE void part_set_rot_v(
    struct part *restrict p, const float rot_v[3]) {
  p->density._rot_v[0] = rot_v[0];
  p->density._rot_v[1] = rot_v[1];
  p->density._rot_v[2] = rot_v[2];
}

/**
 * @brief set velocity curl by index.
 */
static __attribute__((always_inline)) INLINE void part_set_rot_v_ind(
    struct part *restrict p, const size_t i, const float rot_v) {
  p->density._rot_v[i] = rot_v;
}

static __attribute__((always_inline)) INLINE float part_get_f_gradh(
    const struct part *restrict p) {
  return p->force._f_gradh;
}

static __attribute__((always_inline)) INLINE float *part_get_f_gradh_p(
    struct part *restrict p) {
  return &p->force._f_gradh;
}

static __attribute__((always_inline)) INLINE void part_set_f_gradh(
    struct part *restrict p, const float f_gradh) {
  p->force._f_gradh = f_gradh;
}

static __attribute__((always_inline)) INLINE float part_get_pressure(
    const struct part *restrict p) {
  return p->force._pressure;
}

static __attribute__((always_inline)) INLINE float *part_get_pressure_p(
    struct part *restrict p) {
  return &p->force._pressure;
}

static __attribute__((always_inline)) INLINE void part_set_pressure(
    struct part *restrict p, const float pressure) {
  p->force._pressure = pressure;
}

static __attribute__((always_inline)) INLINE float part_get_soundspeed(
    const struct part *restrict p) {
  return p->force._soundspeed;
}

static __attribute__((always_inline)) INLINE float *part_get_soundspeed_p(
    struct part *restrict p) {
  return &p->force._soundspeed;
}

static __attribute__((always_inline)) INLINE void part_set_soundspeed(
    struct part *restrict p, const float soundspeed) {
  p->force._soundspeed = soundspeed;
}

static __attribute__((always_inline)) INLINE float part_get_h_dt(
    const struct part *restrict p) {
  return p->force._h_dt;
}

static __attribute__((always_inline)) INLINE float *part_get_h_dt_p(
    struct part *restrict p) {
  return &p->force._h_dt;
}

static __attribute__((always_inline)) INLINE void part_set_h_dt(
    struct part *restrict p, const float h_dt) {
  p->force._h_dt = h_dt;
}

static __attribute__((always_inline)) INLINE float part_get_balsara(
    const struct part *restrict p) {
  return p->force._balsara;
}

static __attribute__((always_inline)) INLINE float *part_get_balsara_p(
    struct part *restrict p) {
  return &p->force._balsara;
}

static __attribute__((always_inline)) INLINE void part_set_balsara(
    struct part *restrict p, const float balsara) {
  p->force._balsara = balsara;
}

/**
 * @brief Get maximal viscosity parameter alpha over neighbours
 */
static __attribute__((always_inline)) INLINE float part_get_alpha_visc_max_ngb(
    const struct part *restrict p) {
  return p->force._alpha_visc_max_ngb;
}

static __attribute__((always_inline)) INLINE float *
part_get_alpha_visc_max_ngb_p(struct part *restrict p) {
  return &p->force._alpha_visc_max_ngb;
}

/**
 * @brief Set maximal viscosity parameter alpha over neighbours
 */
static __attribute__((always_inline)) INLINE void part_set_alpha_visc_max_ngb(
    struct part *restrict p, const float alpha_visc_max_ngb) {
  p->force._alpha_visc_max_ngb = alpha_visc_max_ngb;
}

/**
 * Use this for read-write access.
 */
static
    __attribute__((always_inline)) INLINE struct adaptive_softening_part_data *
    part_get_adaptive_softening_data_p(struct part *restrict p) {
  return &p->_adaptive_softening_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((
    always_inline)) INLINE const struct adaptive_softening_part_data *
part_get_const_adaptive_softening_data_p(const struct part *restrict p) {
  return &p->_adaptive_softening_data;
}

static __attribute__((always_inline)) INLINE void
part_set_adaptive_softening_data(
    struct part *restrict p,
    const struct adaptive_softening_part_data adaptive_softening_data) {
  p->_adaptive_softening_data = adaptive_softening_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct mhd_part_data *
part_get_mhd_data_p(struct part *restrict p) {
  return &p->_mhd_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct mhd_part_data *
part_get_const_mhd_data_p(const struct part *restrict p) {
  return &p->_mhd_data;
}

static __attribute__((always_inline)) INLINE void part_set_mhd_data(
    struct part *restrict p, const struct mhd_part_data mhd_data) {
  p->_mhd_data = mhd_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct chemistry_part_data *
part_get_chemistry_data_p(struct part *restrict p) {
  return &p->_chemistry_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct chemistry_part_data *
part_get_const_chemistry_data_p(const struct part *restrict p) {
  return &p->_chemistry_data;
}

static __attribute__((always_inline)) INLINE void part_set_chemistry_data(
    struct part *restrict p, const struct chemistry_part_data chemistry_data) {
  p->_chemistry_data = chemistry_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct cooling_part_data *
part_get_cooling_data_p(struct part *restrict p) {
  return &p->_cooling_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct cooling_part_data *
part_get_const_cooling_data_p(const struct part *restrict p) {
  return &p->_cooling_data;
}

static __attribute__((always_inline)) INLINE void part_set_cooling_data(
    struct part *restrict p, const struct cooling_part_data cooling_data) {
  p->_cooling_data = cooling_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct feedback_part_data *
part_get_feedback_data_p(struct part *restrict p) {
  return &p->_feedback_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct feedback_part_data *
part_get_const_feedback_data_p(const struct part *restrict p) {
  return &p->_feedback_data;
}

static __attribute__((always_inline)) INLINE void part_set_feedback_data(
    struct part *restrict p, const struct feedback_part_data feedback_data) {
  p->_feedback_data = feedback_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct black_holes_part_data *
part_get_black_holes_data_p(struct part *restrict p) {
  return &p->_black_holes_data;
}

/**
 * Use this for read-only access.
 */
static
    __attribute__((always_inline)) INLINE const struct black_holes_part_data *
    part_get_const_black_holes_data_p(const struct part *restrict p) {
  return &p->_black_holes_data;
}

static __attribute__((always_inline)) INLINE void part_set_black_holes_data(
    struct part *restrict p,
    const struct black_holes_part_data black_holes_data) {
  p->_black_holes_data = black_holes_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct sink_part_data *
part_get_sink_data_p(struct part *restrict p) {
  return &p->_sink_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct sink_part_data *
part_get_const_sink_data_p(const struct part *restrict p) {
  return &p->_sink_data;
}

static __attribute__((always_inline)) INLINE void part_set_sink_data(
    struct part *restrict p, const struct sink_part_data sink_data) {
  p->_sink_data = sink_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct pressure_floor_part_data *
part_get_pressure_floor_data_p(struct part *restrict p) {
  return &p->_pressure_floor_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((
    always_inline)) INLINE const struct pressure_floor_part_data *
part_get_const_pressure_floor_data_p(const struct part *restrict p) {
  return &p->_pressure_floor_data;
}

static __attribute__((always_inline)) INLINE void part_set_pressure_floor_data(
    struct part *restrict p,
    const struct pressure_floor_part_data pressure_floor_data) {
  p->_pressure_floor_data = pressure_floor_data;
}

/**
 * Use this for read-write access.
 * Don't make it static so we can forward-declare it in rt_struct.h
 */
__attribute__((always_inline)) INLINE struct rt_part_data *part_get_rt_data_p(
    struct part *restrict p) {
  return &p->_rt_data;
}

/**
 * Use this for read-only access.
 * Don't make it static so we can forward-declare it in rt_struct.h
 */
__attribute__((always_inline)) INLINE const struct rt_part_data *
part_get_const_rt_data_p(const struct part *restrict p) {
  return &p->_rt_data;
}

static __attribute__((always_inline)) INLINE void part_set_rt_data(
    struct part *restrict p, const struct rt_part_data rt_data) {
  p->_rt_data = rt_data;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct rt_timestepping_data *
part_get_rt_time_data_p(struct part *restrict p) {
  return &p->_rt_time_data;
}

/**
 * Use this for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct rt_timestepping_data *
part_get_const_rt_time_data_p(const struct part *restrict p) {
  return &p->_rt_time_data;
}

static __attribute__((always_inline)) INLINE void part_set_rt_time_data(
    struct part *restrict p, const struct rt_timestepping_data rt_time_data) {
  p->_rt_time_data = rt_time_data;
}

static __attribute__((always_inline)) INLINE char part_get_depth_h(
    const struct part *restrict p) {
  return p->_depth_h;
}

static __attribute__((always_inline)) INLINE void part_set_depth_h(
    struct part *restrict p, const char depth_h) {
  p->_depth_h = depth_h;
}

static __attribute__((always_inline)) INLINE timebin_t
part_get_time_bin(const struct part *restrict p) {
  return p->_time_bin;
}

static __attribute__((always_inline)) INLINE void part_set_time_bin(
    struct part *restrict p, const timebin_t time_bin) {
  p->_time_bin = time_bin;
}

/**
 * Use this for read-write access.
 */
static __attribute__((always_inline)) INLINE struct timestep_limiter_data *
part_get_limiter_data_p(struct part *restrict p) {
  return &p->_limiter_data;
}

/**
 * Use this for read-only access.
 */
static
    __attribute__((always_inline)) INLINE const struct timestep_limiter_data *
    part_get_const_limiter_data_p(const struct part *restrict p) {
  return &p->_limiter_data;
}

static __attribute__((always_inline)) INLINE void part_set_limiter_data(
    struct part *restrict p, const struct timestep_limiter_data limiter_data) {
  p->_limiter_data = limiter_data;
}

static __attribute__((always_inline)) INLINE integertime_t
part_get_ti_drift(const struct part *restrict p) {
#ifdef SWIFT_DEBUG_CHECKS
  return p->_ti_drift;
#else
  error("Called outside of debug mode!");
  return 0;
#endif
}

static __attribute__((always_inline)) INLINE void part_set_ti_drift(
    struct part *restrict p, const integertime_t ti_drift) {
#ifdef SWIFT_DEBUG_CHECKS
  p->_ti_drift = ti_drift;
#else
  error("Called outside of debug mode!");
#endif
}

static __attribute__((always_inline)) INLINE integertime_t
part_get_ti_kick(const struct part *restrict p) {
#ifdef SWIFT_DEBUG_CHECKS
  return p->_ti_kick;
#else
  error("Called outside of debug mode!");
  return 0;
#endif
}

static __attribute__((always_inline)) INLINE void part_set_ti_kick(
    struct part *restrict p, const integertime_t ti_kick) {
#ifdef SWIFT_DEBUG_CHECKS
  p->_ti_kick = ti_kick;
#else
  error("Called outside of debug mode!");
#endif
}

static __attribute__((always_inline)) INLINE struct fvpm_geometry_struct *
part_get_fvpm_geometry_p(struct part *restrict p) {
  return &p->_geometry;
}

static __attribute__((always_inline)) INLINE const struct fvpm_geometry_struct *
part_get_const_fvpm_geometry_p(const struct part *restrict p) {
  return &p->_geometry;
}

static __attribute__((always_inline)) INLINE void part_set_fvpm_geometry(
    struct part *restrict p, const struct fvpm_geometry_struct geometry) {
  p->_geometry = geometry;
}

#endif /* SWIFT_SPHENIX_HYDRO_PART_H */
