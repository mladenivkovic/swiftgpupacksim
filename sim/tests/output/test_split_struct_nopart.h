
#ifndef SWIFT_HYDRO_PART_TEST_H
#define SWIFT_HYDRO_PART_TEST_H


#ifndef USE_PART_STRUCT_ACCESSORS
#error "USE_PART_STRUCT_ACCESSORS macro not defined. This won't compile."
#endif


#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "hydro_part_arrays_struct.h"

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

#ifdef USE_PART_STRUCT_ACCESSORS
  /*! offset/index of particle in cell particle data array */
  size_t _cell_offset;
#endif

#ifdef USE_PART_STRUCT_ACCESSORS
  /*! pointer to particle data array struct of the cell this particle is located in */
  struct part_arrays* _cell_part_arrays;
#endif

#ifdef SWIFT_DEBUG_CHECKS
  /*! This particle's accessor ID, identical for all structs associated with this particle. */
  long long _accessor_id;
#endif

};

struct density {

  float _d;

  float _e;

#ifdef SWIFT_DEBUG_CHECKS
  /*! This particle's accessor ID, identical for all structs associated with this particle. */
  long long _accessor_id;
#endif

};

struct force {

  float _g;

  float _h;

#ifdef SWIFT_DEBUG_CHECKS
  /*! This particle's accessor ID, identical for all structs associated with this particle. */
  long long _accessor_id;
#endif

};


/**
 * @brief get cell_offset, offset/index of particle in cell particle data array.
 */
static __attribute__((always_inline)) INLINE size_t
  part_get_cell_offset_part_struct(const struct part *restrict p) {
#ifdef USE_PART_STRUCT_ACCESSORS
  return p->_cell_offset;
#else
  return SIZE_MAX;
#endif
}


/**
 * @brief get a pointer to cell_offset, offset/index of particle in cell particle data array.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to cell_offset. If you need read-only access to cell_offset, use part_get_const_cell_offset_p() instead.
 */
static __attribute__((always_inline)) INLINE size_t*
  part_get_cell_offset_p_part_struct(struct part *restrict p) {
#ifdef USE_PART_STRUCT_ACCESSORS
  return &p->_cell_offset;
#else
  return NULL;
#endif
}


/**
 * @brief get read-only access to pointer to cell_offset,
 * offset/index of particle in cell particle data array.
 * If you need write access to cell_offset, use part_get_cell_offset_p() instead.
 */
static __attribute__((always_inline)) INLINE const size_t*
  part_get_const_cell_offset_p_part_struct(const struct part *restrict p) {
#ifdef USE_PART_STRUCT_ACCESSORS
  return &p->_cell_offset;
#else
  return NULL;
#endif
}


/**
 * @brief set the value of cell_offset, offset/index of particle in cell particle data array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_cell_offset_part_struct(struct part *restrict p, const size_t cell_offset) {
#ifdef USE_PART_STRUCT_ACCESSORS
  p->_cell_offset = cell_offset;
#endif
}





/**
 * @brief get cell_part_arrays, pointer to particle data array struct of the cell this particle is located in.
 */
#ifdef USE_PART_STRUCT_ACCESSORS
static __attribute__((always_inline)) INLINE struct part_arrays*
  part_get_cell_part_arrays_part_struct(const struct part *restrict p) {
  return p->_cell_part_arrays;
}
#endif

/**
 * @brief get a pointer to cell_part_arrays, pointer to particle data array struct of the cell this particle is located in.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to cell_part_arrays. If you need read-only access to cell_part_arrays, use part_get_const_cell_part_arrays_p() instead.
 */
#ifdef USE_PART_STRUCT_ACCESSORS
static __attribute__((always_inline)) INLINE struct part_arrays**
  part_get_cell_part_arrays_p_part_struct(struct part *restrict p) {
  return &p->_cell_part_arrays;
}
#endif/**
 * @brief set the value of cell_part_arrays, pointer to particle data array struct of the cell this particle is located in.
 */
#ifdef USE_PART_STRUCT_ACCESSORS
static __attribute__((always_inline)) INLINE void
  part_set_cell_part_arrays_part_struct(struct part *restrict p,  struct part_arrays* cell_part_arrays) {
  p->_cell_part_arrays = cell_part_arrays;
}
#endif






/**
 * @brief get d.
 */
static __attribute__((always_inline)) INLINE float
  part_get_d_part_struct(const struct part *restrict p) {
  const struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  return density_s->_d;
}


/**
 * @brief get a pointer to d.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to d. If you need read-only access to d, use part_get_const_d_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_d_p_part_struct(struct part *restrict p) {
  struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  return &density_s->_d;
}


/**
 * @brief get read-only access to pointer to d.
 * If you need write access to d, use part_get_d_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_d_p_part_struct(const struct part *restrict p) {
  const struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  return &density_s->_d;
}


/**
 * @brief set the value of d.
 */
static __attribute__((always_inline)) INLINE void
  part_set_d_part_struct(struct part *restrict p, const float d) {
  struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  density_s->_d = d;
}





/**
 * @brief get e.
 */
static __attribute__((always_inline)) INLINE float
  part_get_e_part_struct(const struct part *restrict p) {
  const struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  return density_s->_e;
}


/**
 * @brief get a pointer to e.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to e. If you need read-only access to e, use part_get_const_e_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_e_p_part_struct(struct part *restrict p) {
  struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  return &density_s->_e;
}


/**
 * @brief get read-only access to pointer to e.
 * If you need write access to e, use part_get_e_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_e_p_part_struct(const struct part *restrict p) {
  const struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  return &density_s->_e;
}


/**
 * @brief set the value of e.
 */
static __attribute__((always_inline)) INLINE void
  part_set_e_part_struct(struct part *restrict p, const float e) {
  struct density* restrict density_s = p->_cell_part_arrays->_density + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(density_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", density_s->_accessor_id, p->_accessor_id);
#endif
  density_s->_e = e;
}







/**
 * @brief get g.
 */
static __attribute__((always_inline)) INLINE float
  part_get_g_part_struct(const struct part *restrict p) {
  const struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  return force_s->_g;
}


/**
 * @brief get a pointer to g.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to g. If you need read-only access to g, use part_get_const_g_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_g_p_part_struct(struct part *restrict p) {
  struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  return &force_s->_g;
}


/**
 * @brief get read-only access to pointer to g.
 * If you need write access to g, use part_get_g_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_g_p_part_struct(const struct part *restrict p) {
  const struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  return &force_s->_g;
}


/**
 * @brief set the value of g.
 */
static __attribute__((always_inline)) INLINE void
  part_set_g_part_struct(struct part *restrict p, const float g) {
  struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  force_s->_g = g;
}





/**
 * @brief get h.
 */
static __attribute__((always_inline)) INLINE float
  part_get_h_part_struct(const struct part *restrict p) {
  const struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  return force_s->_h;
}


/**
 * @brief get a pointer to h.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to h. If you need read-only access to h, use part_get_const_h_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_h_p_part_struct(struct part *restrict p) {
  struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  return &force_s->_h;
}


/**
 * @brief get read-only access to pointer to h.
 * If you need write access to h, use part_get_h_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_h_p_part_struct(const struct part *restrict p) {
  const struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  return &force_s->_h;
}


/**
 * @brief set the value of h.
 */
static __attribute__((always_inline)) INLINE void
  part_set_h_part_struct(struct part *restrict p, const float h) {
  struct force* restrict force_s = p->_cell_part_arrays->_force + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(force_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", force_s->_accessor_id, p->_accessor_id);
#endif
  force_s->_h = h;
}






#endif /* SWIFT_HYDRO_PART_TEST_H */
