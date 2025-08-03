
#ifndef SWIFT_HYDRO_PART_TEST_H
#define SWIFT_HYDRO_PART_TEST_H

#include <float.h>
#include <limits.h>
#include <stddef.h>

/* workaround for unit tests to produce compilable headers */
#define INLINE inline
typedef unsigned long long integertime_t;
typedef char timebin_t;

struct my_struct{};
struct gpart{
  double x[3];
  float m;
};





struct part {
  /*! my integer */
  int _my_int_arr[4];

  /*! my long */
  long _my_long_arr[4];

  /*! my long long */
  long long _my_longlong_arr[4];

  /*! my float */
  float _my_float_arr[4];

  /*! my double */
  double _my_dble_arr[4];

  /*! my_char */
  char _my_char_arr[4];

  /*! my integertime */
  integertime_t _my_integertime_arr[4];

  /*! my timebin */
  timebin_t _my_timebin_arr[4];

  /*! some externally defined struct */
  struct my_struct _my_external_struct_arr[4];

};

/**
 * @brief get my_int_arr, my integer,
 * for read and write access. For read-only access, use
 * part_get_const_my_int_arr() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int_arr(struct part *restrict p) {
  return p->_my_int_arr;
}

/**
 * @brief get my_int_arr, my integer,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int_arr(const struct part *restrict p) {
  return p->_my_int_arr;
}

/**
 * @brief get my_int_arr, my integer,
 * by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_int_arr[ind];
}

/**
 * @brief set all values of my_int_arr, my integer,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int_arr(struct part *restrict p, const int my_int_arr[4]) {
  p->_my_int_arr[0] = my_int_arr[0];
  p->_my_int_arr[1] = my_int_arr[1];
  p->_my_int_arr[2] = my_int_arr[2];
  p->_my_int_arr[3] = my_int_arr[3];
}

/**
 * @brief set the value of my_int_arr, my integer, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int_arr_ind(struct part *restrict p, const size_t i, const int my_int_arr) {
  p->_my_int_arr[i] = my_int_arr;
}




/**
 * @brief get my_long_arr, my long,
 * for read and write access. For read-only access, use
 * part_get_const_my_long_arr() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long_arr(struct part *restrict p) {
  return p->_my_long_arr;
}

/**
 * @brief get my_long_arr, my long,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long_arr(const struct part *restrict p) {
  return p->_my_long_arr;
}

/**
 * @brief get my_long_arr, my long,
 * by index.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_long_arr[ind];
}

/**
 * @brief set all values of my_long_arr, my long,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long_arr(struct part *restrict p, const long my_long_arr[4]) {
  p->_my_long_arr[0] = my_long_arr[0];
  p->_my_long_arr[1] = my_long_arr[1];
  p->_my_long_arr[2] = my_long_arr[2];
  p->_my_long_arr[3] = my_long_arr[3];
}

/**
 * @brief set the value of my_long_arr, my long, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long_arr_ind(struct part *restrict p, const size_t i, const long my_long_arr) {
  p->_my_long_arr[i] = my_long_arr;
}




/**
 * @brief get my_longlong_arr, my long long,
 * for read and write access. For read-only access, use
 * part_get_const_my_longlong_arr() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong_arr(struct part *restrict p) {
  return p->_my_longlong_arr;
}

/**
 * @brief get my_longlong_arr, my long long,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong_arr(const struct part *restrict p) {
  return p->_my_longlong_arr;
}

/**
 * @brief get my_longlong_arr, my long long,
 * by index.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_longlong_arr[ind];
}

/**
 * @brief set all values of my_longlong_arr, my long long,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong_arr(struct part *restrict p, const long long my_longlong_arr[4]) {
  p->_my_longlong_arr[0] = my_longlong_arr[0];
  p->_my_longlong_arr[1] = my_longlong_arr[1];
  p->_my_longlong_arr[2] = my_longlong_arr[2];
  p->_my_longlong_arr[3] = my_longlong_arr[3];
}

/**
 * @brief set the value of my_longlong_arr, my long long, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong_arr_ind(struct part *restrict p, const size_t i, const long long my_longlong_arr) {
  p->_my_longlong_arr[i] = my_longlong_arr;
}




/**
 * @brief get my_float_arr, my float,
 * for read and write access. For read-only access, use
 * part_get_const_my_float_arr() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float_arr(struct part *restrict p) {
  return p->_my_float_arr;
}

/**
 * @brief get my_float_arr, my float,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float_arr(const struct part *restrict p) {
  return p->_my_float_arr;
}

/**
 * @brief get my_float_arr, my float,
 * by index.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_float_arr[ind];
}

/**
 * @brief set all values of my_float_arr, my float,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float_arr(struct part *restrict p, const float my_float_arr[4]) {
  p->_my_float_arr[0] = my_float_arr[0];
  p->_my_float_arr[1] = my_float_arr[1];
  p->_my_float_arr[2] = my_float_arr[2];
  p->_my_float_arr[3] = my_float_arr[3];
}

/**
 * @brief set the value of my_float_arr, my float, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float_arr_ind(struct part *restrict p, const size_t i, const float my_float_arr) {
  p->_my_float_arr[i] = my_float_arr;
}




/**
 * @brief get my_dble_arr, my double,
 * for read and write access. For read-only access, use
 * part_get_const_my_dble_arr() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble_arr(struct part *restrict p) {
  return p->_my_dble_arr;
}

/**
 * @brief get my_dble_arr, my double,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble_arr(const struct part *restrict p) {
  return p->_my_dble_arr;
}

/**
 * @brief get my_dble_arr, my double,
 * by index.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_dble_arr[ind];
}

/**
 * @brief set all values of my_dble_arr, my double,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble_arr(struct part *restrict p, const double my_dble_arr[4]) {
  p->_my_dble_arr[0] = my_dble_arr[0];
  p->_my_dble_arr[1] = my_dble_arr[1];
  p->_my_dble_arr[2] = my_dble_arr[2];
  p->_my_dble_arr[3] = my_dble_arr[3];
}

/**
 * @brief set the value of my_dble_arr, my double, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble_arr_ind(struct part *restrict p, const size_t i, const double my_dble_arr) {
  p->_my_dble_arr[i] = my_dble_arr;
}




/**
 * @brief get my_char_arr, my_char,
 * for read and write access. For read-only access, use
 * part_get_const_my_char_arr() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char_arr(struct part *restrict p) {
  return p->_my_char_arr;
}

/**
 * @brief get my_char_arr, my_char,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char_arr(const struct part *restrict p) {
  return p->_my_char_arr;
}

/**
 * @brief get my_char_arr, my_char,
 * by index.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_char_arr[ind];
}

/**
 * @brief set all values of my_char_arr, my_char,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char_arr(struct part *restrict p, const char my_char_arr[4]) {
  p->_my_char_arr[0] = my_char_arr[0];
  p->_my_char_arr[1] = my_char_arr[1];
  p->_my_char_arr[2] = my_char_arr[2];
  p->_my_char_arr[3] = my_char_arr[3];
}

/**
 * @brief set the value of my_char_arr, my_char, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char_arr_ind(struct part *restrict p, const size_t i, const char my_char_arr) {
  p->_my_char_arr[i] = my_char_arr;
}




/**
 * @brief get my_integertime_arr, my integertime,
 * for read and write access. For read-only access, use
 * part_get_const_my_integertime_arr() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime_arr(struct part *restrict p) {
  return p->_my_integertime_arr;
}

/**
 * @brief get my_integertime_arr, my integertime,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime_arr(const struct part *restrict p) {
  return p->_my_integertime_arr;
}

/**
 * @brief get my_integertime_arr, my integertime,
 * by index.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_integertime_arr[ind];
}

/**
 * @brief set all values of my_integertime_arr, my integertime,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime_arr(struct part *restrict p, const integertime_t my_integertime_arr[4]) {
  p->_my_integertime_arr[0] = my_integertime_arr[0];
  p->_my_integertime_arr[1] = my_integertime_arr[1];
  p->_my_integertime_arr[2] = my_integertime_arr[2];
  p->_my_integertime_arr[3] = my_integertime_arr[3];
}

/**
 * @brief set the value of my_integertime_arr, my integertime, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime_arr_ind(struct part *restrict p, const size_t i, const integertime_t my_integertime_arr) {
  p->_my_integertime_arr[i] = my_integertime_arr;
}




/**
 * @brief get my_timebin_arr, my timebin,
 * for read and write access. For read-only access, use
 * part_get_const_my_timebin_arr() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin_arr(struct part *restrict p) {
  return p->_my_timebin_arr;
}

/**
 * @brief get my_timebin_arr, my timebin,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin_arr(const struct part *restrict p) {
  return p->_my_timebin_arr;
}

/**
 * @brief get my_timebin_arr, my timebin,
 * by index.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_timebin_arr[ind];
}

/**
 * @brief set all values of my_timebin_arr, my timebin,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin_arr(struct part *restrict p, const timebin_t my_timebin_arr[4]) {
  p->_my_timebin_arr[0] = my_timebin_arr[0];
  p->_my_timebin_arr[1] = my_timebin_arr[1];
  p->_my_timebin_arr[2] = my_timebin_arr[2];
  p->_my_timebin_arr[3] = my_timebin_arr[3];
}

/**
 * @brief set the value of my_timebin_arr, my timebin, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin_arr_ind(struct part *restrict p, const size_t i, const timebin_t my_timebin_arr) {
  p->_my_timebin_arr[i] = my_timebin_arr;
}




/**
 * @brief get my_external_struct_arr, some externally defined struct,
 * for read and write access. For read-only access, use
 * part_get_const_my_external_struct_arr() instead.
 */
static __attribute__((always_inline)) INLINE struct my_struct*
  part_get_my_external_struct_arr(struct part *restrict p) {
  return p->_my_external_struct_arr;
}

/**
 * @brief get my_external_struct_arr, some externally defined struct,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct my_struct*
  part_get_const_my_external_struct_arr(const struct part *restrict p) {
  return p->_my_external_struct_arr;
}

/**
 * @brief get my_external_struct_arr, some externally defined struct,
 * by index.
 */
static __attribute__((always_inline)) INLINE struct my_struct
  part_get_my_external_struct_arr_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_external_struct_arr[ind];
}

/**
 * @brief set all values of my_external_struct_arr, some externally defined struct,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_external_struct_arr(struct part *restrict p, const struct my_struct my_external_struct_arr[4]) {
  p->_my_external_struct_arr[0] = my_external_struct_arr[0];
  p->_my_external_struct_arr[1] = my_external_struct_arr[1];
  p->_my_external_struct_arr[2] = my_external_struct_arr[2];
  p->_my_external_struct_arr[3] = my_external_struct_arr[3];
}

/**
 * @brief set the value of my_external_struct_arr, some externally defined struct, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_external_struct_arr_ind(struct part *restrict p, const size_t i, const struct my_struct my_external_struct_arr) {
  p->_my_external_struct_arr[i] = my_external_struct_arr;
}




#endif /* SWIFT_HYDRO_PART_TEST_H */
