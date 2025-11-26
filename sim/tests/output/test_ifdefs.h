
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

#ifdef DEBUG
  /*! my integer */
  int _my_int;
#endif

#ifdef DEBUG
  /*! my long */
  long _my_long;
#endif

#ifdef DEBUG
  /*! my long long */
  long long _my_longlong;
#endif

#ifdef DEBUG
  /*! my float */
  float _my_float;
#endif

#ifdef DEBUG
  /*! my double */
  double _my_dble;
#endif

#ifdef DEBUG
  /*! my_char */
  char _my_char;
#endif

#ifdef DEBUG
  /*! my integertime */
  integertime_t _my_integertime;
#endif

#ifdef DEBUG
  /*! my timebin */
  timebin_t _my_timebin;
#endif

#ifdef DEBUG
  /*! pointer to externally defined struct */
  struct gpart* _my_pointer_struct;
#endif

#ifdef DEBUG
  /*! pointer to integer array */
  int* _my_pointer;
#endif

#ifdef DEBUG
  /*! my integer */
  int _my_int_arr[4];
#endif

#ifdef DEBUG
  /*! my long */
  long _my_long_arr[4];
#endif

#ifdef DEBUG
  /*! my long long */
  long long _my_longlong_arr[4];
#endif

#ifdef DEBUG
  /*! my float */
  float _my_float_arr[4];
#endif

#ifdef DEBUG
  /*! my double */
  double _my_dble_arr[4];
#endif

#ifdef DEBUG
  /*! my_char */
  char _my_char_arr[4];
#endif

#ifdef DEBUG
  /*! my integertime */
  integertime_t _my_integertime_arr[4];
#endif

#ifdef DEBUG
  /*! my timebin */
  timebin_t _my_timebin_arr[4];
#endif

};


/**
 * @brief get my_int, my integer.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_int;
#else
  return INT_MAX;
#endif
}

/**
 * @brief get a pointer to my_int, my integer.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_int. If you need read-only access to my_int, use part_get_const_my_int_p() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_int;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_int,
 * my integer.
 * If you need write access to my_int, use part_get_my_int_p() instead.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_int;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_int, my integer.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int(struct part *restrict p, const int my_int) {
#ifdef DEBUG
  p->_my_int = my_int;
#endif
}




/**
 * @brief get my_long, my long.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_long;
#else
  return LONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_long, my long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_long. If you need read-only access to my_long, use part_get_const_my_long_p() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_long;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_long,
 * my long.
 * If you need write access to my_long, use part_get_my_long_p() instead.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_long;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_long, my long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long(struct part *restrict p, const long my_long) {
#ifdef DEBUG
  p->_my_long = my_long;
#endif
}




/**
 * @brief get my_longlong, my long long.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_longlong;
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_longlong, my long long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_longlong. If you need read-only access to my_longlong, use part_get_const_my_longlong_p() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_longlong;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_longlong,
 * my long long.
 * If you need write access to my_longlong, use part_get_my_longlong_p() instead.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_longlong;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_longlong, my long long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong(struct part *restrict p, const long long my_longlong) {
#ifdef DEBUG
  p->_my_longlong = my_longlong;
#endif
}




/**
 * @brief get my_float, my float.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_float;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to my_float, my float.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_float. If you need read-only access to my_float, use part_get_const_my_float_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_float;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_float,
 * my float.
 * If you need write access to my_float, use part_get_my_float_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_float;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_float, my float.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float(struct part *restrict p, const float my_float) {
#ifdef DEBUG
  p->_my_float = my_float;
#endif
}




/**
 * @brief get my_dble, my double.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_dble;
#else
  return DBL_MAX;
#endif
}

/**
 * @brief get a pointer to my_dble, my double.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_dble. If you need read-only access to my_dble, use part_get_const_my_dble_p() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_dble;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_dble,
 * my double.
 * If you need write access to my_dble, use part_get_my_dble_p() instead.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_dble;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_dble, my double.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble(struct part *restrict p, const double my_dble) {
#ifdef DEBUG
  p->_my_dble = my_dble;
#endif
}




/**
 * @brief get my_char, my_char.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_char;
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief get a pointer to my_char, my_char.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_char. If you need read-only access to my_char, use part_get_const_my_char_p() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_char;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_char,
 * my_char.
 * If you need write access to my_char, use part_get_my_char_p() instead.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_char;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_char, my_char.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char(struct part *restrict p, const char my_char) {
#ifdef DEBUG
  p->_my_char = my_char;
#endif
}




/**
 * @brief get my_integertime, my integertime.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_integertime;
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_integertime, my integertime.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_integertime. If you need read-only access to my_integertime, use part_get_const_my_integertime_p() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_integertime;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_integertime,
 * my integertime.
 * If you need write access to my_integertime, use part_get_my_integertime_p() instead.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_integertime;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_integertime, my integertime.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime(struct part *restrict p, const integertime_t my_integertime) {
#ifdef DEBUG
  p->_my_integertime = my_integertime;
#endif
}




/**
 * @brief get my_timebin, my timebin.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_timebin;
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief get a pointer to my_timebin, my timebin.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_timebin. If you need read-only access to my_timebin, use part_get_const_my_timebin_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_timebin;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_timebin,
 * my timebin.
 * If you need write access to my_timebin, use part_get_my_timebin_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_timebin;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_timebin, my timebin.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin(struct part *restrict p, const timebin_t my_timebin) {
#ifdef DEBUG
  p->_my_timebin = my_timebin;
#endif
}




/**
 * @brief get my_pointer_struct, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_my_pointer_struct(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_pointer_struct;
#else
  return NULL;
#endif
}

/**
 * @brief get a pointer to my_pointer_struct, pointer to externally defined struct.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer_struct. If you need read-only access to my_pointer_struct, use part_get_const_my_pointer_struct_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_my_pointer_struct_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_pointer_struct;
#else
  return NULL;
#endif
}/**
 * @brief set the value of my_pointer_struct, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer_struct(struct part *restrict p,  struct gpart* my_pointer_struct) {
#ifdef DEBUG
  p->_my_pointer_struct = my_pointer_struct;
#endif
}




/**
 * @brief get my_pointer, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_pointer(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_pointer;
#else
  return NULL;
#endif
}

/**
 * @brief get a pointer to my_pointer, pointer to integer array.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer. If you need read-only access to my_pointer, use part_get_const_my_pointer_p() instead.
 */
static __attribute__((always_inline)) INLINE int**
  part_get_my_pointer_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_my_pointer;
#else
  return NULL;
#endif
}/**
 * @brief set the value of my_pointer, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer(struct part *restrict p,  int* my_pointer) {
#ifdef DEBUG
  p->_my_pointer = my_pointer;
#endif
}




/**
 * @brief get my_int_arr, my integer,
 * for read and write access. For read-only access, use
 * part_get_const_my_int_arr() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_int_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_int_arr, my integer, for read-only access.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_int_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_int_arr, my integer, by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_int_arr[ind];
#else
  return INT_MAX;
#endif
}

/**
 * @brief set all values of my_int_arr, my integer,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int_arr(struct part *restrict p, const int my_int_arr[4]) {
#ifdef DEBUG
  p->_my_int_arr[0] = my_int_arr[0];
  p->_my_int_arr[1] = my_int_arr[1];
  p->_my_int_arr[2] = my_int_arr[2];
  p->_my_int_arr[3] = my_int_arr[3];
#endif
}

/**
 * @brief set the value of my_int_arr, my integer, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int_arr_ind(struct part *restrict p, const size_t i, const int my_int_arr) {
#ifdef DEBUG
  p->_my_int_arr[i] = my_int_arr;
#endif
}




/**
 * @brief get my_long_arr, my long,
 * for read and write access. For read-only access, use
 * part_get_const_my_long_arr() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_long_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_long_arr, my long, for read-only access.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_long_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_long_arr, my long, by index.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_long_arr[ind];
#else
  return LONG_MAX;
#endif
}

/**
 * @brief set all values of my_long_arr, my long,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long_arr(struct part *restrict p, const long my_long_arr[4]) {
#ifdef DEBUG
  p->_my_long_arr[0] = my_long_arr[0];
  p->_my_long_arr[1] = my_long_arr[1];
  p->_my_long_arr[2] = my_long_arr[2];
  p->_my_long_arr[3] = my_long_arr[3];
#endif
}

/**
 * @brief set the value of my_long_arr, my long, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long_arr_ind(struct part *restrict p, const size_t i, const long my_long_arr) {
#ifdef DEBUG
  p->_my_long_arr[i] = my_long_arr;
#endif
}




/**
 * @brief get my_longlong_arr, my long long,
 * for read and write access. For read-only access, use
 * part_get_const_my_longlong_arr() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_longlong_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_longlong_arr, my long long, for read-only access.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_longlong_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_longlong_arr, my long long, by index.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_longlong_arr[ind];
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief set all values of my_longlong_arr, my long long,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong_arr(struct part *restrict p, const long long my_longlong_arr[4]) {
#ifdef DEBUG
  p->_my_longlong_arr[0] = my_longlong_arr[0];
  p->_my_longlong_arr[1] = my_longlong_arr[1];
  p->_my_longlong_arr[2] = my_longlong_arr[2];
  p->_my_longlong_arr[3] = my_longlong_arr[3];
#endif
}

/**
 * @brief set the value of my_longlong_arr, my long long, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong_arr_ind(struct part *restrict p, const size_t i, const long long my_longlong_arr) {
#ifdef DEBUG
  p->_my_longlong_arr[i] = my_longlong_arr;
#endif
}




/**
 * @brief get my_float_arr, my float,
 * for read and write access. For read-only access, use
 * part_get_const_my_float_arr() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_float_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_float_arr, my float, for read-only access.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_float_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_float_arr, my float, by index.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_float_arr[ind];
#else
  return FLT_MAX;
#endif
}

/**
 * @brief set all values of my_float_arr, my float,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float_arr(struct part *restrict p, const float my_float_arr[4]) {
#ifdef DEBUG
  p->_my_float_arr[0] = my_float_arr[0];
  p->_my_float_arr[1] = my_float_arr[1];
  p->_my_float_arr[2] = my_float_arr[2];
  p->_my_float_arr[3] = my_float_arr[3];
#endif
}

/**
 * @brief set the value of my_float_arr, my float, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float_arr_ind(struct part *restrict p, const size_t i, const float my_float_arr) {
#ifdef DEBUG
  p->_my_float_arr[i] = my_float_arr;
#endif
}




/**
 * @brief get my_dble_arr, my double,
 * for read and write access. For read-only access, use
 * part_get_const_my_dble_arr() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_dble_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_dble_arr, my double, for read-only access.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_dble_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_dble_arr, my double, by index.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_dble_arr[ind];
#else
  return DBL_MAX;
#endif
}

/**
 * @brief set all values of my_dble_arr, my double,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble_arr(struct part *restrict p, const double my_dble_arr[4]) {
#ifdef DEBUG
  p->_my_dble_arr[0] = my_dble_arr[0];
  p->_my_dble_arr[1] = my_dble_arr[1];
  p->_my_dble_arr[2] = my_dble_arr[2];
  p->_my_dble_arr[3] = my_dble_arr[3];
#endif
}

/**
 * @brief set the value of my_dble_arr, my double, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble_arr_ind(struct part *restrict p, const size_t i, const double my_dble_arr) {
#ifdef DEBUG
  p->_my_dble_arr[i] = my_dble_arr;
#endif
}




/**
 * @brief get my_char_arr, my_char,
 * for read and write access. For read-only access, use
 * part_get_const_my_char_arr() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_char_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_char_arr, my_char, for read-only access.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_char_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_char_arr, my_char, by index.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_char_arr[ind];
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief set all values of my_char_arr, my_char,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char_arr(struct part *restrict p, const char my_char_arr[4]) {
#ifdef DEBUG
  p->_my_char_arr[0] = my_char_arr[0];
  p->_my_char_arr[1] = my_char_arr[1];
  p->_my_char_arr[2] = my_char_arr[2];
  p->_my_char_arr[3] = my_char_arr[3];
#endif
}

/**
 * @brief set the value of my_char_arr, my_char, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char_arr_ind(struct part *restrict p, const size_t i, const char my_char_arr) {
#ifdef DEBUG
  p->_my_char_arr[i] = my_char_arr;
#endif
}




/**
 * @brief get my_integertime_arr, my integertime,
 * for read and write access. For read-only access, use
 * part_get_const_my_integertime_arr() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_integertime_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_integertime_arr, my integertime, for read-only access.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_integertime_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_integertime_arr, my integertime, by index.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_integertime_arr[ind];
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief set all values of my_integertime_arr, my integertime,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime_arr(struct part *restrict p, const integertime_t my_integertime_arr[4]) {
#ifdef DEBUG
  p->_my_integertime_arr[0] = my_integertime_arr[0];
  p->_my_integertime_arr[1] = my_integertime_arr[1];
  p->_my_integertime_arr[2] = my_integertime_arr[2];
  p->_my_integertime_arr[3] = my_integertime_arr[3];
#endif
}

/**
 * @brief set the value of my_integertime_arr, my integertime, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime_arr_ind(struct part *restrict p, const size_t i, const integertime_t my_integertime_arr) {
#ifdef DEBUG
  p->_my_integertime_arr[i] = my_integertime_arr;
#endif
}




/**
 * @brief get my_timebin_arr, my timebin,
 * for read and write access. For read-only access, use
 * part_get_const_my_timebin_arr() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin_arr(struct part *restrict p) {
#ifdef DEBUG
  return p->_my_timebin_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_timebin_arr, my timebin, for read-only access.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin_arr(const struct part *restrict p) {
#ifdef DEBUG
  return p->_my_timebin_arr;
#else
  return NULL;
#endif
}

/**
 * @brief get my_timebin_arr, my timebin, by index.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin_arr_ind(const struct part *restrict p, const size_t ind) {
#ifdef DEBUG
  return p->_my_timebin_arr[ind];
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief set all values of my_timebin_arr, my timebin,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin_arr(struct part *restrict p, const timebin_t my_timebin_arr[4]) {
#ifdef DEBUG
  p->_my_timebin_arr[0] = my_timebin_arr[0];
  p->_my_timebin_arr[1] = my_timebin_arr[1];
  p->_my_timebin_arr[2] = my_timebin_arr[2];
  p->_my_timebin_arr[3] = my_timebin_arr[3];
#endif
}

/**
 * @brief set the value of my_timebin_arr, my timebin, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin_arr_ind(struct part *restrict p, const size_t i, const timebin_t my_timebin_arr) {
#ifdef DEBUG
  p->_my_timebin_arr[i] = my_timebin_arr;
#endif
}



#endif /* SWIFT_HYDRO_PART_TEST_H */
