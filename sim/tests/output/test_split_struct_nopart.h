
#ifndef SWIFT_HYDRO_PART_TEST_H
#define SWIFT_HYDRO_PART_TEST_H

#include <float.h>
#include <limits.h>
#include <stddef.h>

#include "hydro_part_data_struct.h"

/* workaround for unit tests to produce compilable headers */
#define INLINE inline
#define DEBUG
typedef unsigned long long integertime_t;
typedef char timebin_t;

struct my_struct{};
struct gpart{
  double x[3];
  float m;
};





struct part {

  /*! offset/index of particle in cell particle data array */
  size_t _cell_offset;

  /*! pointer to particle data array struct of the cell this particle is located in */
  struct hydro_part_data* _cell_part_data;

};

struct density {

  float _d;

  float _e;

};

struct force {

  float _g;

  float _h;

};


/**
 * @brief get d.
 */
static __attribute__((always_inline)) INLINE float
  part_get_d(const struct part *restrict p) {
  const struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  return density_s->_d;
}

/**
 * @brief get a pointer to d.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to d. If you need read-only access to d, use part_get_const_d_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_d_p(struct part *restrict p) {
  struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  return &density_s->_d;
}

/**
 * @brief get read-only access to pointer to d.
 * If you need write access to d, use part_get_d_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_d_p(const struct part *restrict p) {
  const struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  return &density_s->_d;
}

/**
 * @brief set the value of d.
 */
static __attribute__((always_inline)) INLINE void
  part_set_d(struct part *restrict p, const float d) {
  struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  density_s->_d = d;
}




/**
 * @brief get e.
 */
static __attribute__((always_inline)) INLINE float
  part_get_e(const struct part *restrict p) {
  const struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  return density_s->_e;
}

/**
 * @brief get a pointer to e.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to e. If you need read-only access to e, use part_get_const_e_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_e_p(struct part *restrict p) {
  struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  return &density_s->_e;
}

/**
 * @brief get read-only access to pointer to e.
 * If you need write access to e, use part_get_e_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_e_p(const struct part *restrict p) {
  const struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  return &density_s->_e;
}

/**
 * @brief set the value of e.
 */
static __attribute__((always_inline)) INLINE void
  part_set_e(struct part *restrict p, const float e) {
  struct density* density_s = p->_cell_part_data->_density + p->_cell_offset;
  density_s->_e = e;
}




/**
 * @brief get g.
 */
static __attribute__((always_inline)) INLINE float
  part_get_g(const struct part *restrict p) {
  const struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  return force_s->_g;
}

/**
 * @brief get a pointer to g.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to g. If you need read-only access to g, use part_get_const_g_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_g_p(struct part *restrict p) {
  struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  return &force_s->_g;
}

/**
 * @brief get read-only access to pointer to g.
 * If you need write access to g, use part_get_g_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_g_p(const struct part *restrict p) {
  const struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  return &force_s->_g;
}

/**
 * @brief set the value of g.
 */
static __attribute__((always_inline)) INLINE void
  part_set_g(struct part *restrict p, const float g) {
  struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  force_s->_g = g;
}




/**
 * @brief get h.
 */
static __attribute__((always_inline)) INLINE float
  part_get_h(const struct part *restrict p) {
  const struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  return force_s->_h;
}

/**
 * @brief get a pointer to h.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to h. If you need read-only access to h, use part_get_const_h_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_h_p(struct part *restrict p) {
  struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  return &force_s->_h;
}

/**
 * @brief get read-only access to pointer to h.
 * If you need write access to h, use part_get_h_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_h_p(const struct part *restrict p) {
  const struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  return &force_s->_h;
}

/**
 * @brief set the value of h.
 */
static __attribute__((always_inline)) INLINE void
  part_set_h(struct part *restrict p, const float h) {
  struct force* force_s = p->_cell_part_data->_force + p->_cell_offset;
  force_s->_h = h;
}



#endif /* SWIFT_HYDRO_PART_TEST_H */
