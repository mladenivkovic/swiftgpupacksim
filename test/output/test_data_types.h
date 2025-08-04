
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

  /*! my integer */
  int _my_int;

  /*! my long */
  long _my_long;

  /*! my long long */
  long long _my_longlong;

  /*! my float */
  float _my_float;

  /*! my double */
  double _my_dble;

  /*! my_char */
  char _my_char;

  /*! my integertime */
  integertime_t _my_integertime;

  /*! my timebin */
  timebin_t _my_timebin;

  /*! some externally defined struct */
  struct my_struct _my_external_struct;

  /*! pointer to externally defined struct */
  struct gpart* _my_pointer_struct;

  /*! pointer to integer array */
  int* _my_pointer;

};


/**
 * @brief get my_int, my integer.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int(const struct part *restrict p) {
  return p->_my_int;
}

/**
 * @brief get a pointer to my_int, my integer.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_int. If you need read-only access to my_int, use part_get_const_my_int_p() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int_p(struct part *restrict p) {
  return &p->_my_int;
}

/**
 * @brief get read-only access to pointer to my_int,
 * my integer.
 * If you need write access to my_int, use part_get_my_int_p() instead.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int_p(const struct part *restrict p) {
  return &p->_my_int;
}

/**
 * @brief set the value of my_int, my integer.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int(struct part *restrict p, const int my_int) {
  p->_my_int = my_int;
}




/**
 * @brief get my_long, my long.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long(const struct part *restrict p) {
  return p->_my_long;
}

/**
 * @brief get a pointer to my_long, my long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_long. If you need read-only access to my_long, use part_get_const_my_long_p() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long_p(struct part *restrict p) {
  return &p->_my_long;
}

/**
 * @brief get read-only access to pointer to my_long,
 * my long.
 * If you need write access to my_long, use part_get_my_long_p() instead.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long_p(const struct part *restrict p) {
  return &p->_my_long;
}

/**
 * @brief set the value of my_long, my long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long(struct part *restrict p, const long my_long) {
  p->_my_long = my_long;
}




/**
 * @brief get my_longlong, my long long.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong(const struct part *restrict p) {
  return p->_my_longlong;
}

/**
 * @brief get a pointer to my_longlong, my long long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_longlong. If you need read-only access to my_longlong, use part_get_const_my_longlong_p() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong_p(struct part *restrict p) {
  return &p->_my_longlong;
}

/**
 * @brief get read-only access to pointer to my_longlong,
 * my long long.
 * If you need write access to my_longlong, use part_get_my_longlong_p() instead.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong_p(const struct part *restrict p) {
  return &p->_my_longlong;
}

/**
 * @brief set the value of my_longlong, my long long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong(struct part *restrict p, const long long my_longlong) {
  p->_my_longlong = my_longlong;
}




/**
 * @brief get my_float, my float.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float(const struct part *restrict p) {
  return p->_my_float;
}

/**
 * @brief get a pointer to my_float, my float.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_float. If you need read-only access to my_float, use part_get_const_my_float_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float_p(struct part *restrict p) {
  return &p->_my_float;
}

/**
 * @brief get read-only access to pointer to my_float,
 * my float.
 * If you need write access to my_float, use part_get_my_float_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float_p(const struct part *restrict p) {
  return &p->_my_float;
}

/**
 * @brief set the value of my_float, my float.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float(struct part *restrict p, const float my_float) {
  p->_my_float = my_float;
}




/**
 * @brief get my_dble, my double.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble(const struct part *restrict p) {
  return p->_my_dble;
}

/**
 * @brief get a pointer to my_dble, my double.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_dble. If you need read-only access to my_dble, use part_get_const_my_dble_p() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble_p(struct part *restrict p) {
  return &p->_my_dble;
}

/**
 * @brief get read-only access to pointer to my_dble,
 * my double.
 * If you need write access to my_dble, use part_get_my_dble_p() instead.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble_p(const struct part *restrict p) {
  return &p->_my_dble;
}

/**
 * @brief set the value of my_dble, my double.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble(struct part *restrict p, const double my_dble) {
  p->_my_dble = my_dble;
}




/**
 * @brief get my_char, my_char.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char(const struct part *restrict p) {
  return p->_my_char;
}

/**
 * @brief get a pointer to my_char, my_char.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_char. If you need read-only access to my_char, use part_get_const_my_char_p() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char_p(struct part *restrict p) {
  return &p->_my_char;
}

/**
 * @brief get read-only access to pointer to my_char,
 * my_char.
 * If you need write access to my_char, use part_get_my_char_p() instead.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char_p(const struct part *restrict p) {
  return &p->_my_char;
}

/**
 * @brief set the value of my_char, my_char.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char(struct part *restrict p, const char my_char) {
  p->_my_char = my_char;
}




/**
 * @brief get my_integertime, my integertime.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime(const struct part *restrict p) {
  return p->_my_integertime;
}

/**
 * @brief get a pointer to my_integertime, my integertime.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_integertime. If you need read-only access to my_integertime, use part_get_const_my_integertime_p() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime_p(struct part *restrict p) {
  return &p->_my_integertime;
}

/**
 * @brief get read-only access to pointer to my_integertime,
 * my integertime.
 * If you need write access to my_integertime, use part_get_my_integertime_p() instead.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime_p(const struct part *restrict p) {
  return &p->_my_integertime;
}

/**
 * @brief set the value of my_integertime, my integertime.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime(struct part *restrict p, const integertime_t my_integertime) {
  p->_my_integertime = my_integertime;
}




/**
 * @brief get my_timebin, my timebin.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin(const struct part *restrict p) {
  return p->_my_timebin;
}

/**
 * @brief get a pointer to my_timebin, my timebin.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_timebin. If you need read-only access to my_timebin, use part_get_const_my_timebin_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin_p(struct part *restrict p) {
  return &p->_my_timebin;
}

/**
 * @brief get read-only access to pointer to my_timebin,
 * my timebin.
 * If you need write access to my_timebin, use part_get_my_timebin_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin_p(const struct part *restrict p) {
  return &p->_my_timebin;
}

/**
 * @brief set the value of my_timebin, my timebin.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin(struct part *restrict p, const timebin_t my_timebin) {
  p->_my_timebin = my_timebin;
}




/**
 * @brief get my_external_struct, some externally defined struct.
 */
static __attribute__((always_inline)) INLINE struct my_struct
  part_get_my_external_struct(const struct part *restrict p) {
  return p->_my_external_struct;
}

/**
 * @brief get a pointer to my_external_struct, some externally defined struct.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_external_struct. If you need read-only access to my_external_struct, use part_get_const_my_external_struct_p() instead.
 */
static __attribute__((always_inline)) INLINE struct my_struct*
  part_get_my_external_struct_p(struct part *restrict p) {
  return &p->_my_external_struct;
}

/**
 * @brief get read-only access to pointer to my_external_struct,
 * some externally defined struct.
 * If you need write access to my_external_struct, use part_get_my_external_struct_p() instead.
 */
static __attribute__((always_inline)) INLINE const struct my_struct*
  part_get_const_my_external_struct_p(const struct part *restrict p) {
  return &p->_my_external_struct;
}

/**
 * @brief set the value of my_external_struct, some externally defined struct.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_external_struct(struct part *restrict p, const struct my_struct my_external_struct) {
  p->_my_external_struct = my_external_struct;
}




/**
 * @brief get my_pointer_struct, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_my_pointer_struct(const struct part *restrict p) {
  return p->_my_pointer_struct;
}

/**
 * @brief get a pointer to my_pointer_struct, pointer to externally defined struct.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer_struct. If you need read-only access to my_pointer_struct, use part_get_const_my_pointer_struct_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_my_pointer_struct_p(struct part *restrict p) {
  return &p->_my_pointer_struct;
}/**
 * @brief set the value of my_pointer_struct, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer_struct(struct part *restrict p,  struct gpart* my_pointer_struct) {
  p->_my_pointer_struct = my_pointer_struct;
}




/**
 * @brief get my_pointer, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_pointer(const struct part *restrict p) {
  return p->_my_pointer;
}

/**
 * @brief get a pointer to my_pointer, pointer to integer array.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer. If you need read-only access to my_pointer, use part_get_const_my_pointer_p() instead.
 */
static __attribute__((always_inline)) INLINE int**
  part_get_my_pointer_p(struct part *restrict p) {
  return &p->_my_pointer;
}/**
 * @brief set the value of my_pointer, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer(struct part *restrict p,  int* my_pointer) {
  p->_my_pointer = my_pointer;
}



#endif /* SWIFT_HYDRO_PART_TEST_H */
