
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
  int _my_int[4];

  /*! my long */
  long _my_long[4];

  /*! my long long */
  long long _my_longlong[4];

  /*! my float */
  float _my_float[4];

  /*! my double */
  double _my_dble[4];

  /*! my_char */
  char _my_char[4];

  /*! my integertime */
  integertime_t _my_integertime[4];

  /*! my timebin */
  timebin_t _my_timebin[4];

  /*! some externally defined struct */
  struct my_struct _my_external_struct[4];

};

/**
 * @brief get my_int, my integer,
 * for read and write access. For read-only access, use
 * part_get_const_my_int() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int(struct part *restrict p) {
  return p->_my_int;
}

/**
 * @brief get my_int, my integer,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int(const struct part *restrict p) {
  return p->_my_int;
}

/**
 * @brief get my_int, my integer,
 * by index.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_int[ind];
}

/**
 * @brief set all values of my_int, my integer,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int(struct part *restrict p, const int my_int[4]) {
  p->_my_int[0] = my_int[0];
  p->_my_int[1] = my_int[1];
  p->_my_int[2] = my_int[2];
  p->_my_int[3] = my_int[3];
}

/**
 * @brief set the value of my_int, my integer, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int_ind(struct part *restrict p, const size_t i, const int my_int) {
  p->_my_int[i] = my_int;
}




/**
 * @brief get my_long, my long,
 * for read and write access. For read-only access, use
 * part_get_const_my_long() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long(struct part *restrict p) {
  return p->_my_long;
}

/**
 * @brief get my_long, my long,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long(const struct part *restrict p) {
  return p->_my_long;
}

/**
 * @brief get my_long, my long,
 * by index.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_long[ind];
}

/**
 * @brief set all values of my_long, my long,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long(struct part *restrict p, const long my_long[4]) {
  p->_my_long[0] = my_long[0];
  p->_my_long[1] = my_long[1];
  p->_my_long[2] = my_long[2];
  p->_my_long[3] = my_long[3];
}

/**
 * @brief set the value of my_long, my long, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long_ind(struct part *restrict p, const size_t i, const long my_long) {
  p->_my_long[i] = my_long;
}




/**
 * @brief get my_longlong, my long long,
 * for read and write access. For read-only access, use
 * part_get_const_my_longlong() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong(struct part *restrict p) {
  return p->_my_longlong;
}

/**
 * @brief get my_longlong, my long long,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong(const struct part *restrict p) {
  return p->_my_longlong;
}

/**
 * @brief get my_longlong, my long long,
 * by index.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_longlong[ind];
}

/**
 * @brief set all values of my_longlong, my long long,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong(struct part *restrict p, const long long my_longlong[4]) {
  p->_my_longlong[0] = my_longlong[0];
  p->_my_longlong[1] = my_longlong[1];
  p->_my_longlong[2] = my_longlong[2];
  p->_my_longlong[3] = my_longlong[3];
}

/**
 * @brief set the value of my_longlong, my long long, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong_ind(struct part *restrict p, const size_t i, const long long my_longlong) {
  p->_my_longlong[i] = my_longlong;
}




/**
 * @brief get my_float, my float,
 * for read and write access. For read-only access, use
 * part_get_const_my_float() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float(struct part *restrict p) {
  return p->_my_float;
}

/**
 * @brief get my_float, my float,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float(const struct part *restrict p) {
  return p->_my_float;
}

/**
 * @brief get my_float, my float,
 * by index.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_float[ind];
}

/**
 * @brief set all values of my_float, my float,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float(struct part *restrict p, const float my_float[4]) {
  p->_my_float[0] = my_float[0];
  p->_my_float[1] = my_float[1];
  p->_my_float[2] = my_float[2];
  p->_my_float[3] = my_float[3];
}

/**
 * @brief set the value of my_float, my float, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float_ind(struct part *restrict p, const size_t i, const float my_float) {
  p->_my_float[i] = my_float;
}




/**
 * @brief get my_dble, my double,
 * for read and write access. For read-only access, use
 * part_get_const_my_dble() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble(struct part *restrict p) {
  return p->_my_dble;
}

/**
 * @brief get my_dble, my double,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble(const struct part *restrict p) {
  return p->_my_dble;
}

/**
 * @brief get my_dble, my double,
 * by index.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_dble[ind];
}

/**
 * @brief set all values of my_dble, my double,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble(struct part *restrict p, const double my_dble[4]) {
  p->_my_dble[0] = my_dble[0];
  p->_my_dble[1] = my_dble[1];
  p->_my_dble[2] = my_dble[2];
  p->_my_dble[3] = my_dble[3];
}

/**
 * @brief set the value of my_dble, my double, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble_ind(struct part *restrict p, const size_t i, const double my_dble) {
  p->_my_dble[i] = my_dble;
}




/**
 * @brief get my_char, my_char,
 * for read and write access. For read-only access, use
 * part_get_const_my_char() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char(struct part *restrict p) {
  return p->_my_char;
}

/**
 * @brief get my_char, my_char,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char(const struct part *restrict p) {
  return p->_my_char;
}

/**
 * @brief get my_char, my_char,
 * by index.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_char[ind];
}

/**
 * @brief set all values of my_char, my_char,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char(struct part *restrict p, const char my_char[4]) {
  p->_my_char[0] = my_char[0];
  p->_my_char[1] = my_char[1];
  p->_my_char[2] = my_char[2];
  p->_my_char[3] = my_char[3];
}

/**
 * @brief set the value of my_char, my_char, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char_ind(struct part *restrict p, const size_t i, const char my_char) {
  p->_my_char[i] = my_char;
}




/**
 * @brief get my_integertime, my integertime,
 * for read and write access. For read-only access, use
 * part_get_const_my_integertime() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime(struct part *restrict p) {
  return p->_my_integertime;
}

/**
 * @brief get my_integertime, my integertime,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime(const struct part *restrict p) {
  return p->_my_integertime;
}

/**
 * @brief get my_integertime, my integertime,
 * by index.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_integertime[ind];
}

/**
 * @brief set all values of my_integertime, my integertime,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime(struct part *restrict p, const integertime_t my_integertime[4]) {
  p->_my_integertime[0] = my_integertime[0];
  p->_my_integertime[1] = my_integertime[1];
  p->_my_integertime[2] = my_integertime[2];
  p->_my_integertime[3] = my_integertime[3];
}

/**
 * @brief set the value of my_integertime, my integertime, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime_ind(struct part *restrict p, const size_t i, const integertime_t my_integertime) {
  p->_my_integertime[i] = my_integertime;
}




/**
 * @brief get my_timebin, my timebin,
 * for read and write access. For read-only access, use
 * part_get_const_my_timebin() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin(struct part *restrict p) {
  return p->_my_timebin;
}

/**
 * @brief get my_timebin, my timebin,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin(const struct part *restrict p) {
  return p->_my_timebin;
}

/**
 * @brief get my_timebin, my timebin,
 * by index.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_timebin[ind];
}

/**
 * @brief set all values of my_timebin, my timebin,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin(struct part *restrict p, const timebin_t my_timebin[4]) {
  p->_my_timebin[0] = my_timebin[0];
  p->_my_timebin[1] = my_timebin[1];
  p->_my_timebin[2] = my_timebin[2];
  p->_my_timebin[3] = my_timebin[3];
}

/**
 * @brief set the value of my_timebin, my timebin, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin_ind(struct part *restrict p, const size_t i, const timebin_t my_timebin) {
  p->_my_timebin[i] = my_timebin;
}




/**
 * @brief get my_external_struct, some externally defined struct,
 * for read and write access. For read-only access, use
 * part_get_const_my_external_struct() instead.
 */
static __attribute__((always_inline)) INLINE struct my_struct*
  part_get_my_external_struct(struct part *restrict p) {
  return p->_my_external_struct;
}

/**
 * @brief get my_external_struct, some externally defined struct,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const struct my_struct*
  part_get_const_my_external_struct(const struct part *restrict p) {
  return p->_my_external_struct;
}

/**
 * @brief get my_external_struct, some externally defined struct,
 * by index.
 */
static __attribute__((always_inline)) INLINE struct my_struct
  part_get_my_external_struct_ind(const struct part *restrict p, const size_t ind) {
  return p->_my_external_struct[ind];
}

/**
 * @brief set all values of my_external_struct, some externally defined struct,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_external_struct(struct part *restrict p, const struct my_struct my_external_struct[4]) {
  p->_my_external_struct[0] = my_external_struct[0];
  p->_my_external_struct[1] = my_external_struct[1];
  p->_my_external_struct[2] = my_external_struct[2];
  p->_my_external_struct[3] = my_external_struct[3];
}

/**
 * @brief set the value of my_external_struct, some externally defined struct, by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_external_struct_ind(struct part *restrict p, const size_t i, const struct my_struct my_external_struct) {
  p->_my_external_struct[i] = my_external_struct;
}




#endif /* SWIFT_HYDRO_PART_TEST_H */
