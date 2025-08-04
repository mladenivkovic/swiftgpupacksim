
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

struct part2 {

  /*! my integer */
  int _my_int2;

  /*! my long */
  long _my_long2;

  /*! my long long */
  long long _my_longlong2;

  /*! my float */
  float _my_float2;

  /*! my double */
  double _my_dble2;

  /*! my_char */
  char _my_char2;

  /*! my integertime */
  integertime_t _my_integertime2;

  /*! my timebin */
  timebin_t _my_timebin2;

  /*! some externally defined struct */
  struct my_struct _my_external_struct2;

  /*! pointer to externally defined struct */
  struct gpart* _my_pointer_struct2;

  /*! pointer to integer array */
  int* _my_pointer2;

};

struct part3 {

#ifdef DEBUG
  /*! my integer */
  int _my_int3;
#endif

#ifdef DEBUG
  /*! my long */
  long _my_long3;
#endif

#ifdef DEBUG
  /*! my long long */
  long long _my_longlong3;
#endif

#ifdef DEBUG
  /*! my float */
  float _my_float3;
#endif

#ifdef DEBUG
  /*! my double */
  double _my_dble3;
#endif

#ifdef DEBUG
  /*! my_char */
  char _my_char3;
#endif

#ifdef DEBUG
  /*! my integertime */
  integertime_t _my_integertime3;
#endif

#ifdef DEBUG
  /*! my timebin */
  timebin_t _my_timebin3;
#endif

#ifdef DEBUG
  /*! pointer to externally defined struct */
  struct gpart* _my_pointer_struct3;
#endif

#ifdef DEBUG
  /*! pointer to integer array */
  int* _my_pointer3;
#endif

};

struct part4 {

#ifdef NODEBUG
  /*! my integer */
  int _my_int4;
#endif

#ifdef NODEBUG
  /*! my long */
  long _my_long4;
#endif

#ifdef NODEBUG
  /*! my long long */
  long long _my_longlong4;
#endif

#ifdef NODEBUG
  /*! my float */
  float _my_float4;
#endif

#ifdef NODEBUG
  /*! my double */
  double _my_dble4;
#endif

#ifdef NODEBUG
  /*! my_char */
  char _my_char4;
#endif

#ifdef NODEBUG
  /*! my integertime */
  integertime_t _my_integertime4;
#endif

#ifdef NODEBUG
  /*! my timebin */
  timebin_t _my_timebin4;
#endif

#ifdef NODEBUG
  /*! pointer to externally defined struct */
  struct gpart* _my_pointer_struct4;
#endif

#ifdef NODEBUG
  /*! pointer to integer array */
  int* _my_pointer4;
#endif

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




/**
 * @brief get my_int2, my integer.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_int2;
}

/**
 * @brief get a pointer to my_int2, my integer.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_int2. If you need read-only access to my_int2, use part_get_const_my_int2_p() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_int2;
}

/**
 * @brief get read-only access to pointer to my_int2,
 * my integer.
 * If you need write access to my_int2, use part_get_my_int2_p() instead.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_int2;
}

/**
 * @brief set the value of my_int2, my integer.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int2(struct part *restrict p, const int my_int2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_int2 = my_int2;
}




/**
 * @brief get my_long2, my long.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_long2;
}

/**
 * @brief get a pointer to my_long2, my long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_long2. If you need read-only access to my_long2, use part_get_const_my_long2_p() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_long2;
}

/**
 * @brief get read-only access to pointer to my_long2,
 * my long.
 * If you need write access to my_long2, use part_get_my_long2_p() instead.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_long2;
}

/**
 * @brief set the value of my_long2, my long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long2(struct part *restrict p, const long my_long2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_long2 = my_long2;
}




/**
 * @brief get my_longlong2, my long long.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_longlong2;
}

/**
 * @brief get a pointer to my_longlong2, my long long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_longlong2. If you need read-only access to my_longlong2, use part_get_const_my_longlong2_p() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_longlong2;
}

/**
 * @brief get read-only access to pointer to my_longlong2,
 * my long long.
 * If you need write access to my_longlong2, use part_get_my_longlong2_p() instead.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_longlong2;
}

/**
 * @brief set the value of my_longlong2, my long long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong2(struct part *restrict p, const long long my_longlong2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_longlong2 = my_longlong2;
}




/**
 * @brief get my_float2, my float.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_float2;
}

/**
 * @brief get a pointer to my_float2, my float.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_float2. If you need read-only access to my_float2, use part_get_const_my_float2_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_float2;
}

/**
 * @brief get read-only access to pointer to my_float2,
 * my float.
 * If you need write access to my_float2, use part_get_my_float2_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_float2;
}

/**
 * @brief set the value of my_float2, my float.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float2(struct part *restrict p, const float my_float2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_float2 = my_float2;
}




/**
 * @brief get my_dble2, my double.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_dble2;
}

/**
 * @brief get a pointer to my_dble2, my double.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_dble2. If you need read-only access to my_dble2, use part_get_const_my_dble2_p() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_dble2;
}

/**
 * @brief get read-only access to pointer to my_dble2,
 * my double.
 * If you need write access to my_dble2, use part_get_my_dble2_p() instead.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_dble2;
}

/**
 * @brief set the value of my_dble2, my double.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble2(struct part *restrict p, const double my_dble2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_dble2 = my_dble2;
}




/**
 * @brief get my_char2, my_char.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_char2;
}

/**
 * @brief get a pointer to my_char2, my_char.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_char2. If you need read-only access to my_char2, use part_get_const_my_char2_p() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_char2;
}

/**
 * @brief get read-only access to pointer to my_char2,
 * my_char.
 * If you need write access to my_char2, use part_get_my_char2_p() instead.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_char2;
}

/**
 * @brief set the value of my_char2, my_char.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char2(struct part *restrict p, const char my_char2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_char2 = my_char2;
}




/**
 * @brief get my_integertime2, my integertime.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_integertime2;
}

/**
 * @brief get a pointer to my_integertime2, my integertime.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_integertime2. If you need read-only access to my_integertime2, use part_get_const_my_integertime2_p() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_integertime2;
}

/**
 * @brief get read-only access to pointer to my_integertime2,
 * my integertime.
 * If you need write access to my_integertime2, use part_get_my_integertime2_p() instead.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_integertime2;
}

/**
 * @brief set the value of my_integertime2, my integertime.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime2(struct part *restrict p, const integertime_t my_integertime2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_integertime2 = my_integertime2;
}




/**
 * @brief get my_timebin2, my timebin.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_timebin2;
}

/**
 * @brief get a pointer to my_timebin2, my timebin.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_timebin2. If you need read-only access to my_timebin2, use part_get_const_my_timebin2_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_timebin2;
}

/**
 * @brief get read-only access to pointer to my_timebin2,
 * my timebin.
 * If you need write access to my_timebin2, use part_get_my_timebin2_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_timebin2;
}

/**
 * @brief set the value of my_timebin2, my timebin.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin2(struct part *restrict p, const timebin_t my_timebin2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_timebin2 = my_timebin2;
}




/**
 * @brief get my_external_struct2, some externally defined struct.
 */
static __attribute__((always_inline)) INLINE struct my_struct
  part_get_my_external_struct2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_external_struct2;
}

/**
 * @brief get a pointer to my_external_struct2, some externally defined struct.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_external_struct2. If you need read-only access to my_external_struct2, use part_get_const_my_external_struct2_p() instead.
 */
static __attribute__((always_inline)) INLINE struct my_struct*
  part_get_my_external_struct2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_external_struct2;
}

/**
 * @brief get read-only access to pointer to my_external_struct2,
 * some externally defined struct.
 * If you need write access to my_external_struct2, use part_get_my_external_struct2_p() instead.
 */
static __attribute__((always_inline)) INLINE const struct my_struct*
  part_get_const_my_external_struct2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_external_struct2;
}

/**
 * @brief set the value of my_external_struct2, some externally defined struct.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_external_struct2(struct part *restrict p, const struct my_struct my_external_struct2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_external_struct2 = my_external_struct2;
}




/**
 * @brief get my_pointer_struct2, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_my_pointer_struct2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_pointer_struct2;
}

/**
 * @brief get a pointer to my_pointer_struct2, pointer to externally defined struct.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer_struct2. If you need read-only access to my_pointer_struct2, use part_get_const_my_pointer_struct2_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_my_pointer_struct2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_pointer_struct2;
}/**
 * @brief set the value of my_pointer_struct2, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer_struct2(struct part *restrict p,  struct gpart* my_pointer_struct2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_pointer_struct2 = my_pointer_struct2;
}




/**
 * @brief get my_pointer2, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_pointer2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_my_pointer2;
}

/**
 * @brief get a pointer to my_pointer2, pointer to integer array.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer2. If you need read-only access to my_pointer2, use part_get_const_my_pointer2_p() instead.
 */
static __attribute__((always_inline)) INLINE int**
  part_get_my_pointer2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_my_pointer2;
}/**
 * @brief set the value of my_pointer2, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer2(struct part *restrict p,  int* my_pointer2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_my_pointer2 = my_pointer2;
}




/**
 * @brief get my_int3, my integer.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_int3;
#else
  return INT_MAX;
#endif
}

/**
 * @brief get a pointer to my_int3, my integer.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_int3. If you need read-only access to my_int3, use part_get_const_my_int3_p() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_int3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_int3,
 * my integer.
 * If you need write access to my_int3, use part_get_my_int3_p() instead.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_int3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_int3, my integer.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int3(struct part *restrict p, const int my_int3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_int3 = my_int3;
#endif
}




/**
 * @brief get my_long3, my long.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_long3;
#else
  return LONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_long3, my long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_long3. If you need read-only access to my_long3, use part_get_const_my_long3_p() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_long3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_long3,
 * my long.
 * If you need write access to my_long3, use part_get_my_long3_p() instead.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_long3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_long3, my long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long3(struct part *restrict p, const long my_long3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_long3 = my_long3;
#endif
}




/**
 * @brief get my_longlong3, my long long.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_longlong3;
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_longlong3, my long long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_longlong3. If you need read-only access to my_longlong3, use part_get_const_my_longlong3_p() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_longlong3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_longlong3,
 * my long long.
 * If you need write access to my_longlong3, use part_get_my_longlong3_p() instead.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_longlong3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_longlong3, my long long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong3(struct part *restrict p, const long long my_longlong3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_longlong3 = my_longlong3;
#endif
}




/**
 * @brief get my_float3, my float.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_float3;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to my_float3, my float.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_float3. If you need read-only access to my_float3, use part_get_const_my_float3_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_float3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_float3,
 * my float.
 * If you need write access to my_float3, use part_get_my_float3_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_float3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_float3, my float.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float3(struct part *restrict p, const float my_float3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_float3 = my_float3;
#endif
}




/**
 * @brief get my_dble3, my double.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_dble3;
#else
  return DBL_MAX;
#endif
}

/**
 * @brief get a pointer to my_dble3, my double.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_dble3. If you need read-only access to my_dble3, use part_get_const_my_dble3_p() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_dble3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_dble3,
 * my double.
 * If you need write access to my_dble3, use part_get_my_dble3_p() instead.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_dble3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_dble3, my double.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble3(struct part *restrict p, const double my_dble3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_dble3 = my_dble3;
#endif
}




/**
 * @brief get my_char3, my_char.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_char3;
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief get a pointer to my_char3, my_char.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_char3. If you need read-only access to my_char3, use part_get_const_my_char3_p() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_char3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_char3,
 * my_char.
 * If you need write access to my_char3, use part_get_my_char3_p() instead.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_char3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_char3, my_char.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char3(struct part *restrict p, const char my_char3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_char3 = my_char3;
#endif
}




/**
 * @brief get my_integertime3, my integertime.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_integertime3;
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_integertime3, my integertime.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_integertime3. If you need read-only access to my_integertime3, use part_get_const_my_integertime3_p() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_integertime3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_integertime3,
 * my integertime.
 * If you need write access to my_integertime3, use part_get_my_integertime3_p() instead.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_integertime3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_integertime3, my integertime.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime3(struct part *restrict p, const integertime_t my_integertime3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_integertime3 = my_integertime3;
#endif
}




/**
 * @brief get my_timebin3, my timebin.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_timebin3;
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief get a pointer to my_timebin3, my timebin.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_timebin3. If you need read-only access to my_timebin3, use part_get_const_my_timebin3_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_timebin3;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_timebin3,
 * my timebin.
 * If you need write access to my_timebin3, use part_get_my_timebin3_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin3_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_timebin3;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_timebin3, my timebin.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin3(struct part *restrict p, const timebin_t my_timebin3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_timebin3 = my_timebin3;
#endif
}




/**
 * @brief get my_pointer_struct3, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_my_pointer_struct3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_pointer_struct3;
#else
  return NULL;
#endif
}

/**
 * @brief get a pointer to my_pointer_struct3, pointer to externally defined struct.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer_struct3. If you need read-only access to my_pointer_struct3, use part_get_const_my_pointer_struct3_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_my_pointer_struct3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_pointer_struct3;
#else
  return NULL;
#endif
}/**
 * @brief set the value of my_pointer_struct3, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer_struct3(struct part *restrict p,  struct gpart* my_pointer_struct3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_pointer_struct3 = my_pointer_struct3;
#endif
}




/**
 * @brief get my_pointer3, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_pointer3(const struct part *restrict p) {
#ifdef DEBUG
  const struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return part3_s->_my_pointer3;
#else
  return NULL;
#endif
}

/**
 * @brief get a pointer to my_pointer3, pointer to integer array.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer3. If you need read-only access to my_pointer3, use part_get_const_my_pointer3_p() instead.
 */
static __attribute__((always_inline)) INLINE int**
  part_get_my_pointer3_p(struct part *restrict p) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  return &part3_s->_my_pointer3;
#else
  return NULL;
#endif
}/**
 * @brief set the value of my_pointer3, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer3(struct part *restrict p,  int* my_pointer3) {
#ifdef DEBUG
  struct part3* part3_s = p->_cell_part_data->_part3 + p->_cell_offset;
  part3_s->_my_pointer3 = my_pointer3;
#endif
}




/**
 * @brief get my_int4, my integer.
 */
static __attribute__((always_inline)) INLINE int
  part_get_my_int4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_int4;
#else
  return INT_MAX;
#endif
}

/**
 * @brief get a pointer to my_int4, my integer.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_int4. If you need read-only access to my_int4, use part_get_const_my_int4_p() instead.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_int4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_int4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_int4,
 * my integer.
 * If you need write access to my_int4, use part_get_my_int4_p() instead.
 */
static __attribute__((always_inline)) INLINE const int*
  part_get_const_my_int4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_int4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_int4, my integer.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_int4(struct part *restrict p, const int my_int4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_int4 = my_int4;
#endif
}




/**
 * @brief get my_long4, my long.
 */
static __attribute__((always_inline)) INLINE long
  part_get_my_long4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_long4;
#else
  return LONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_long4, my long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_long4. If you need read-only access to my_long4, use part_get_const_my_long4_p() instead.
 */
static __attribute__((always_inline)) INLINE long*
  part_get_my_long4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_long4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_long4,
 * my long.
 * If you need write access to my_long4, use part_get_my_long4_p() instead.
 */
static __attribute__((always_inline)) INLINE const long*
  part_get_const_my_long4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_long4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_long4, my long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_long4(struct part *restrict p, const long my_long4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_long4 = my_long4;
#endif
}




/**
 * @brief get my_longlong4, my long long.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_my_longlong4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_longlong4;
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_longlong4, my long long.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_longlong4. If you need read-only access to my_longlong4, use part_get_const_my_longlong4_p() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_my_longlong4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_longlong4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_longlong4,
 * my long long.
 * If you need write access to my_longlong4, use part_get_my_longlong4_p() instead.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_my_longlong4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_longlong4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_longlong4, my long long.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_longlong4(struct part *restrict p, const long long my_longlong4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_longlong4 = my_longlong4;
#endif
}




/**
 * @brief get my_float4, my float.
 */
static __attribute__((always_inline)) INLINE float
  part_get_my_float4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_float4;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to my_float4, my float.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_float4. If you need read-only access to my_float4, use part_get_const_my_float4_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_my_float4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_float4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_float4,
 * my float.
 * If you need write access to my_float4, use part_get_my_float4_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_my_float4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_float4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_float4, my float.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_float4(struct part *restrict p, const float my_float4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_float4 = my_float4;
#endif
}




/**
 * @brief get my_dble4, my double.
 */
static __attribute__((always_inline)) INLINE double
  part_get_my_dble4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_dble4;
#else
  return DBL_MAX;
#endif
}

/**
 * @brief get a pointer to my_dble4, my double.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_dble4. If you need read-only access to my_dble4, use part_get_const_my_dble4_p() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_my_dble4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_dble4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_dble4,
 * my double.
 * If you need write access to my_dble4, use part_get_my_dble4_p() instead.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_my_dble4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_dble4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_dble4, my double.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_dble4(struct part *restrict p, const double my_dble4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_dble4 = my_dble4;
#endif
}




/**
 * @brief get my_char4, my_char.
 */
static __attribute__((always_inline)) INLINE char
  part_get_my_char4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_char4;
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief get a pointer to my_char4, my_char.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_char4. If you need read-only access to my_char4, use part_get_const_my_char4_p() instead.
 */
static __attribute__((always_inline)) INLINE char*
  part_get_my_char4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_char4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_char4,
 * my_char.
 * If you need write access to my_char4, use part_get_my_char4_p() instead.
 */
static __attribute__((always_inline)) INLINE const char*
  part_get_const_my_char4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_char4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_char4, my_char.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_char4(struct part *restrict p, const char my_char4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_char4 = my_char4;
#endif
}




/**
 * @brief get my_integertime4, my integertime.
 */
static __attribute__((always_inline)) INLINE integertime_t
  part_get_my_integertime4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_integertime4;
#else
  return LLONG_MAX;
#endif
}

/**
 * @brief get a pointer to my_integertime4, my integertime.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_integertime4. If you need read-only access to my_integertime4, use part_get_const_my_integertime4_p() instead.
 */
static __attribute__((always_inline)) INLINE integertime_t*
  part_get_my_integertime4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_integertime4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_integertime4,
 * my integertime.
 * If you need write access to my_integertime4, use part_get_my_integertime4_p() instead.
 */
static __attribute__((always_inline)) INLINE const integertime_t*
  part_get_const_my_integertime4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_integertime4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_integertime4, my integertime.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_integertime4(struct part *restrict p, const integertime_t my_integertime4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_integertime4 = my_integertime4;
#endif
}




/**
 * @brief get my_timebin4, my timebin.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_my_timebin4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_timebin4;
#else
  return CHAR_MAX;
#endif
}

/**
 * @brief get a pointer to my_timebin4, my timebin.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_timebin4. If you need read-only access to my_timebin4, use part_get_const_my_timebin4_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_my_timebin4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_timebin4;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to my_timebin4,
 * my timebin.
 * If you need write access to my_timebin4, use part_get_my_timebin4_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_my_timebin4_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_timebin4;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of my_timebin4, my timebin.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_timebin4(struct part *restrict p, const timebin_t my_timebin4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_timebin4 = my_timebin4;
#endif
}




/**
 * @brief get my_pointer_struct4, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_my_pointer_struct4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_pointer_struct4;
#else
  return NULL;
#endif
}

/**
 * @brief get a pointer to my_pointer_struct4, pointer to externally defined struct.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer_struct4. If you need read-only access to my_pointer_struct4, use part_get_const_my_pointer_struct4_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_my_pointer_struct4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_pointer_struct4;
#else
  return NULL;
#endif
}/**
 * @brief set the value of my_pointer_struct4, pointer to externally defined struct.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer_struct4(struct part *restrict p,  struct gpart* my_pointer_struct4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_pointer_struct4 = my_pointer_struct4;
#endif
}




/**
 * @brief get my_pointer4, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE int*
  part_get_my_pointer4(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return part4_s->_my_pointer4;
#else
  return NULL;
#endif
}

/**
 * @brief get a pointer to my_pointer4, pointer to integer array.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to my_pointer4. If you need read-only access to my_pointer4, use part_get_const_my_pointer4_p() instead.
 */
static __attribute__((always_inline)) INLINE int**
  part_get_my_pointer4_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  return &part4_s->_my_pointer4;
#else
  return NULL;
#endif
}/**
 * @brief set the value of my_pointer4, pointer to integer array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_my_pointer4(struct part *restrict p,  int* my_pointer4) {
#ifdef NODEBUG
  struct part4* part4_s = p->_cell_part_data->_part4 + p->_cell_offset;
  part4_s->_my_pointer4 = my_pointer4;
#endif
}



#endif /* SWIFT_HYDRO_PART_TEST_H */
