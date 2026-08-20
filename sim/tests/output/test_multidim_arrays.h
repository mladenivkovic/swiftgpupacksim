
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

/* Some definitions and declarations to make the test programs compile */
#ifndef MY_ARRAY_SIZE
#define MY_ARRAY_SIZE 4
#endif

#define NDIM1 2
#define NDIM2 3
#define NDIM3 4

#define INLINE inline
#define DEBUG
typedef unsigned long long integertime_t;
typedef char timebin_t;

enum color {
  red = 0,
  green,
  blue,
  purple,
  white,
  black
  };

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

  /*! my integer */
  int _my_2D_arr[4][4];

  int _my_macro_2D_arr[NDIM1][NDIM2];

  int _my_3D_arr[4][4][4];

  int _my_macro_3D_arr[NDIM1][2][NDIM3];

#ifdef DEBUG
  int _my_3D_arr2[3][3][3];
#endif

#ifdef SWIFT_DEBUG_CHECKS
  /*! This particle's accessor ID, identical for all structs associated with this particle. */
  long long _accessor_id;
#endif

};

struct part2 {

#ifdef DEBUG
  int _my_3D_arr3[3][3][3];
#endif

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
 * @brief get my_2D_arr, my integer, by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_2D_arr_ind_part_struct(const struct part *restrict p, const int i, const int j) {
  return p->_my_2D_arr[i][j];
}

/**
 * @brief set all values of my_2D_arr, my integer,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_2D_arr_part_struct(struct part *restrict p, const int my_2D_arr[4][4]) {

  
    for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++){
        p->_my_2D_arr[i][j] = my_2D_arr[i][j];
      }
    }
  
}

/**
 * @brief set the value of my_2D_arr, my integer, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_2D_arr_ind_part_struct(struct part *restrict p, const int i,  const int j, int val) {
  p->_my_2D_arr[i][j] = val;
}






/**
 * @brief get my_macro_2D_arr by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_macro_2D_arr_ind_part_struct(const struct part *restrict p, const int i, const int j) {
  return p->_my_macro_2D_arr[i][j];
}

/**
 * @brief set all values of my_macro_2D_arr,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_macro_2D_arr_part_struct(struct part *restrict p, const int my_macro_2D_arr[NDIM1][NDIM2]) {

  
    for (int i = 0; i < NDIM1; i++){
      for (int j = 0; j < NDIM2; j++){
        p->_my_macro_2D_arr[i][j] = my_macro_2D_arr[i][j];
      }
    }
  
}

/**
 * @brief set the value of my_macro_2D_arr by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_macro_2D_arr_ind_part_struct(struct part *restrict p, const int i,  const int j, int val) {
  p->_my_macro_2D_arr[i][j] = val;
}






/**
 * @brief get my_3D_arr by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_3D_arr_ind_part_struct(const struct part *restrict p, const int i, const int j, const int k) {
  return p->_my_3D_arr[i][j][k];
}

/**
 * @brief set all values of my_3D_arr,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_3D_arr_part_struct(struct part *restrict p, const int my_3D_arr[4][4][4]) {

  
    for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++){
        for (int k = 0; k < 4; k++){
          p->_my_3D_arr[i][j][k] = my_3D_arr[i][j][k];
        }
      }
    }
  
}

/**
 * @brief set the value of my_3D_arr by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_3D_arr_ind_part_struct(struct part *restrict p, const int i,  const int j,  const int k, int val) {
  p->_my_3D_arr[i][j][k] = val;
}






/**
 * @brief get my_macro_3D_arr by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_macro_3D_arr_ind_part_struct(const struct part *restrict p, const int i, const int j, const int k) {
  return p->_my_macro_3D_arr[i][j][k];
}

/**
 * @brief set all values of my_macro_3D_arr,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_macro_3D_arr_part_struct(struct part *restrict p, const int my_macro_3D_arr[NDIM1][2][NDIM3]) {

  
    for (int i = 0; i < NDIM1; i++){
      for (int j = 0; j < 2; j++){
        for (int k = 0; k < NDIM3; k++){
          p->_my_macro_3D_arr[i][j][k] = my_macro_3D_arr[i][j][k];
        }
      }
    }
  
}

/**
 * @brief set the value of my_macro_3D_arr by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_macro_3D_arr_ind_part_struct(struct part *restrict p, const int i,  const int j,  const int k, int val) {
  p->_my_macro_3D_arr[i][j][k] = val;
}






/**
 * @brief get my_3D_arr2 by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_3D_arr2_ind_part_struct(const struct part *restrict p, const int i, const int j, const int k) {
#ifdef DEBUG
  return p->_my_3D_arr2[i][j][k];
#else
  return INT_MAX;
#endif
}

/**
 * @brief set all values of my_3D_arr2,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_3D_arr2_part_struct(struct part *restrict p, const int my_3D_arr2[3][3][3]) {
#ifdef DEBUG

  
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++){
        for (int k = 0; k < 3; k++){
          p->_my_3D_arr2[i][j][k] = my_3D_arr2[i][j][k];
        }
      }
    }
  
#endif
}

/**
 * @brief set the value of my_3D_arr2 by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_3D_arr2_ind_part_struct(struct part *restrict p, const int i,  const int j,  const int k, int val) {
#ifdef DEBUG
  p->_my_3D_arr2[i][j][k] = val;
#endif
}








/**
 * @brief get my_3D_arr3 by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_3D_arr3_ind_part_struct(const struct part *restrict p, const int i, const int j, const int k) {
#ifdef DEBUG
  const struct part2* restrict part2_s = p->_cell_part_arrays->_part2 + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(part2_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", part2_s->_accessor_id, p->_accessor_id);
#endif
  return part2_s->_my_3D_arr3[i][j][k];
#else
  return INT_MAX;
#endif
}

/**
 * @brief set all values of my_3D_arr3,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_3D_arr3_part_struct(struct part *restrict p, const int my_3D_arr3[3][3][3]) {
#ifdef DEBUG
  struct part2* restrict part2_s = p->_cell_part_arrays->_part2 + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(part2_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", part2_s->_accessor_id, p->_accessor_id);
#endif

  
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++){
        for (int k = 0; k < 3; k++){      part2_s->_my_3D_arr3[i][j][k] = my_3D_arr3[i][j][k];
  
        }
  
      }
  
    }
  
#endif
}

/**
 * @brief set the value of my_3D_arr3 by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_3D_arr3_ind_part_struct(struct part *restrict p, const int i,  const int j,  const int k, int val) {
#ifdef DEBUG
  struct part2* restrict part2_s = p->_cell_part_arrays->_part2 + p->_cell_offset;
#ifdef SWIFT_DEBUG_CHECKS
  /* Forbid ID = 0 to prevent false positives by forgotten initialisation */
  swift_assert(p->_accessor_id != 0);
  /* Make sure we're accessing the correct data */
  if(part2_s->_accessor_id != p->_accessor_id)
    error("Accessor IDs not equal: %lld %lld", part2_s->_accessor_id, p->_accessor_id);
      #endif
  part2_s->_my_3D_arr3[i][j][k] = val;
#endif
}





#endif /* SWIFT_HYDRO_PART_TEST_H */
