
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

  struct {

    float _a;

    float _b[3];

#ifdef DEBUG
    long long _c;
#endif

   }_st1;

  struct {

    double _d;

    struct {

      struct gpart* _gps;

      timebin_t _ts;

     }_nested1;

   }_st2;

  union {

    float _au;

#ifdef DEBUG
    float _bu;
#endif

#ifdef NODEBUG
    float _cu;
#endif

   };

  union {

    struct gp* _gpu;

    struct {

      float _du;

      float _eu;

     }_ppu;

   };

};

struct part2 {

  struct {

    float _a2;

    float _b2[3];

#ifdef DEBUG
    long long _c2;
#endif

   }_st3;

  struct {

    double _d2;

    struct {

      struct gpart* _gps2;

      timebin_t _ts2;

     }_nested12;

   }_st4;

  union {

    float _au2;

#ifdef DEBUG
    float _bu2;
#endif

#ifdef NODEBUG
    float _cu2;
#endif

   };

  union {

    struct gp* _gpu2;

    struct {

      float _du2;

      float _eu2;

     }_ppu2;

   };

};


/**
 * @brief get a.
 */
static __attribute__((always_inline)) INLINE float
  part_get_a(const struct part *restrict p) {
  return p->_st1._a;
}

/**
 * @brief get a pointer to a.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to a. If you need read-only access to a, use part_get_const_a_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_a_p(struct part *restrict p) {
  return &p->_st1._a;
}

/**
 * @brief get read-only access to pointer to a.
 * If you need write access to a, use part_get_a_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_a_p(const struct part *restrict p) {
  return &p->_st1._a;
}

/**
 * @brief set the value of a.
 */
static __attribute__((always_inline)) INLINE void
  part_set_a(struct part *restrict p, const float a) {
  p->_st1._a = a;
}


/**
 * @brief get b
 * for read and write access. For read-only access, use
 * part_get_const_b() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_b(struct part *restrict p) {
  return p->_st1._b;
}

/**
 * @brief get b for read-only access.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_b(const struct part *restrict p) {
  return p->_st1._b;
}

/**
 * @brief get b by index.
 */
static __attribute__((always_inline)) INLINE float
  part_get_b_ind(const struct part *restrict p, const size_t ind) {
  return p->_st1._b[ind];
}

/**
 * @brief set all values of b,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_b(struct part *restrict p, const float b[3]) {
  p->_st1._b[0] = b[0];
  p->_st1._b[1] = b[1];
  p->_st1._b[2] = b[2];
}

/**
 * @brief set the value of b by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_b_ind(struct part *restrict p, const size_t i, const float b) {
  p->_st1._b[i] = b;
}


/**
 * @brief get c.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_c(const struct part *restrict p) {
#ifdef DEBUG
  return p->_st1._c;
#else
  return 0;
#endif
}

/**
 * @brief get a pointer to c.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to c. If you need read-only access to c, use part_get_const_c_p() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_c_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_st1._c;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to c.
 * If you need write access to c, use part_get_c_p() instead.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_c_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_st1._c;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of c.
 */
static __attribute__((always_inline)) INLINE void
  part_set_c(struct part *restrict p, const long long c) {
#ifdef DEBUG
  p->_st1._c = c;
#endif
}




/**
 * @brief get d.
 */
static __attribute__((always_inline)) INLINE double
  part_get_d(const struct part *restrict p) {
  return p->_st2._d;
}

/**
 * @brief get a pointer to d.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to d. If you need read-only access to d, use part_get_const_d_p() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_d_p(struct part *restrict p) {
  return &p->_st2._d;
}

/**
 * @brief get read-only access to pointer to d.
 * If you need write access to d, use part_get_d_p() instead.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_d_p(const struct part *restrict p) {
  return &p->_st2._d;
}

/**
 * @brief set the value of d.
 */
static __attribute__((always_inline)) INLINE void
  part_set_d(struct part *restrict p, const double d) {
  p->_st2._d = d;
}


/**
 * @brief get gps.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_gps(const struct part *restrict p) {
  return p->_st2._nested1._gps;
}

/**
 * @brief get a pointer to gps.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to gps. If you need read-only access to gps, use part_get_const_gps_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_gps_p(struct part *restrict p) {
  return &p->_st2._nested1._gps;
}/**
 * @brief set the value of gps.
 */
static __attribute__((always_inline)) INLINE void
  part_set_gps(struct part *restrict p,  struct gpart* gps) {
  p->_st2._nested1._gps = gps;
}


/**
 * @brief get ts.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_ts(const struct part *restrict p) {
  return p->_st2._nested1._ts;
}

/**
 * @brief get a pointer to ts.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to ts. If you need read-only access to ts, use part_get_const_ts_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_ts_p(struct part *restrict p) {
  return &p->_st2._nested1._ts;
}

/**
 * @brief get read-only access to pointer to ts.
 * If you need write access to ts, use part_get_ts_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_ts_p(const struct part *restrict p) {
  return &p->_st2._nested1._ts;
}

/**
 * @brief set the value of ts.
 */
static __attribute__((always_inline)) INLINE void
  part_set_ts(struct part *restrict p, const timebin_t ts) {
  p->_st2._nested1._ts = ts;
}




/**
 * @brief get au.
 */
static __attribute__((always_inline)) INLINE float
  part_get_au(const struct part *restrict p) {
  return p->_au;
}

/**
 * @brief get a pointer to au.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to au. If you need read-only access to au, use part_get_const_au_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_au_p(struct part *restrict p) {
  return &p->_au;
}

/**
 * @brief get read-only access to pointer to au.
 * If you need write access to au, use part_get_au_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_au_p(const struct part *restrict p) {
  return &p->_au;
}

/**
 * @brief set the value of au.
 */
static __attribute__((always_inline)) INLINE void
  part_set_au(struct part *restrict p, const float au) {
  p->_au = au;
}


/**
 * @brief get bu.
 */
static __attribute__((always_inline)) INLINE float
  part_get_bu(const struct part *restrict p) {
#ifdef DEBUG
  return p->_bu;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to bu.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to bu. If you need read-only access to bu, use part_get_const_bu_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_bu_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_bu;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to bu.
 * If you need write access to bu, use part_get_bu_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_bu_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_bu;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of bu.
 */
static __attribute__((always_inline)) INLINE void
  part_set_bu(struct part *restrict p, const float bu) {
#ifdef DEBUG
  p->_bu = bu;
#endif
}


/**
 * @brief get cu.
 */
static __attribute__((always_inline)) INLINE float
  part_get_cu(const struct part *restrict p) {
#ifdef NODEBUG
  return p->_cu;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to cu.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to cu. If you need read-only access to cu, use part_get_const_cu_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_cu_p(struct part *restrict p) {
#ifdef NODEBUG
  return &p->_cu;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to cu.
 * If you need write access to cu, use part_get_cu_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_cu_p(const struct part *restrict p) {
#ifdef NODEBUG
  return &p->_cu;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of cu.
 */
static __attribute__((always_inline)) INLINE void
  part_set_cu(struct part *restrict p, const float cu) {
#ifdef NODEBUG
  p->_cu = cu;
#endif
}




/**
 * @brief get gpu.
 */
static __attribute__((always_inline)) INLINE struct gp*
  part_get_gpu(const struct part *restrict p) {
  return p->_gpu;
}

/**
 * @brief get a pointer to gpu.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to gpu. If you need read-only access to gpu, use part_get_const_gpu_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gp**
  part_get_gpu_p(struct part *restrict p) {
  return &p->_gpu;
}/**
 * @brief set the value of gpu.
 */
static __attribute__((always_inline)) INLINE void
  part_set_gpu(struct part *restrict p,  struct gp* gpu) {
  p->_gpu = gpu;
}


/**
 * @brief get du.
 */
static __attribute__((always_inline)) INLINE float
  part_get_du(const struct part *restrict p) {
  return p->_ppu._du;
}

/**
 * @brief get a pointer to du.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to du. If you need read-only access to du, use part_get_const_du_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_du_p(struct part *restrict p) {
  return &p->_ppu._du;
}

/**
 * @brief get read-only access to pointer to du.
 * If you need write access to du, use part_get_du_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_du_p(const struct part *restrict p) {
  return &p->_ppu._du;
}

/**
 * @brief set the value of du.
 */
static __attribute__((always_inline)) INLINE void
  part_set_du(struct part *restrict p, const float du) {
  p->_ppu._du = du;
}


/**
 * @brief get eu.
 */
static __attribute__((always_inline)) INLINE float
  part_get_eu(const struct part *restrict p) {
  return p->_ppu._eu;
}

/**
 * @brief get a pointer to eu.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to eu. If you need read-only access to eu, use part_get_const_eu_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_eu_p(struct part *restrict p) {
  return &p->_ppu._eu;
}

/**
 * @brief get read-only access to pointer to eu.
 * If you need write access to eu, use part_get_eu_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_eu_p(const struct part *restrict p) {
  return &p->_ppu._eu;
}

/**
 * @brief set the value of eu.
 */
static __attribute__((always_inline)) INLINE void
  part_set_eu(struct part *restrict p, const float eu) {
  p->_ppu._eu = eu;
}




/**
 * @brief get a2.
 */
static __attribute__((always_inline)) INLINE float
  part_get_a2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st3._a2;
}

/**
 * @brief get a pointer to a2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to a2. If you need read-only access to a2, use part_get_const_a2_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_a2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st3._a2;
}

/**
 * @brief get read-only access to pointer to a2.
 * If you need write access to a2, use part_get_a2_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_a2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st3._a2;
}

/**
 * @brief set the value of a2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_a2(struct part *restrict p, const float a2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_st3._a2 = a2;
}


/**
 * @brief get b2
 * for read and write access. For read-only access, use
 * part_get_const_b2() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_b2(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st3._b2;
}

/**
 * @brief get b2 for read-only access.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_b2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st3._b2;
}

/**
 * @brief get b2 by index.
 */
static __attribute__((always_inline)) INLINE float
  part_get_b2_ind(const struct part *restrict p, const size_t ind) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st3._b2[ind];
}

/**
 * @brief set all values of b2,
 * from an array.
 */
static __attribute__((always_inline)) INLINE void
  part_set_b2(struct part *restrict p, const float b2[3]) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_st3._b2[0] = b2[0];
  part2_s->_st3._b2[1] = b2[1];
  part2_s->_st3._b2[2] = b2[2];
}

/**
 * @brief set the value of b2 by index.
 */
static __attribute__((always_inline)) INLINE void
  part_set_b2_ind(struct part *restrict p, const size_t i, const float b2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_st3._b2[i] = b2;
}


/**
 * @brief get c2.
 */
static __attribute__((always_inline)) INLINE long long
  part_get_c2(const struct part *restrict p) {
#ifdef DEBUG
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st3._c2;
#else
  return 0;
#endif
}

/**
 * @brief get a pointer to c2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to c2. If you need read-only access to c2, use part_get_const_c2_p() instead.
 */
static __attribute__((always_inline)) INLINE long long*
  part_get_c2_p(struct part *restrict p) {
#ifdef DEBUG
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st3._c2;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to c2.
 * If you need write access to c2, use part_get_c2_p() instead.
 */
static __attribute__((always_inline)) INLINE const long long*
  part_get_const_c2_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st3._c2;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of c2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_c2(struct part *restrict p, const long long c2) {
#ifdef DEBUG
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_st3._c2 = c2;
#endif
}




/**
 * @brief get d2.
 */
static __attribute__((always_inline)) INLINE double
  part_get_d2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st4._d2;
}

/**
 * @brief get a pointer to d2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to d2. If you need read-only access to d2, use part_get_const_d2_p() instead.
 */
static __attribute__((always_inline)) INLINE double*
  part_get_d2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st4._d2;
}

/**
 * @brief get read-only access to pointer to d2.
 * If you need write access to d2, use part_get_d2_p() instead.
 */
static __attribute__((always_inline)) INLINE const double*
  part_get_const_d2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st4._d2;
}

/**
 * @brief set the value of d2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_d2(struct part *restrict p, const double d2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_st4._d2 = d2;
}


/**
 * @brief get gps2.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_gps2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st4._nested12._gps2;
}

/**
 * @brief get a pointer to gps2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to gps2. If you need read-only access to gps2, use part_get_const_gps2_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_gps2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st4._nested12._gps2;
}/**
 * @brief set the value of gps2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_gps2(struct part *restrict p,  struct gpart* gps2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_st4._nested12._gps2 = gps2;
}


/**
 * @brief get ts2.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_ts2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_st4._nested12._ts2;
}

/**
 * @brief get a pointer to ts2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to ts2. If you need read-only access to ts2, use part_get_const_ts2_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_ts2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st4._nested12._ts2;
}

/**
 * @brief get read-only access to pointer to ts2.
 * If you need write access to ts2, use part_get_ts2_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_ts2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_st4._nested12._ts2;
}

/**
 * @brief set the value of ts2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_ts2(struct part *restrict p, const timebin_t ts2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_st4._nested12._ts2 = ts2;
}




/**
 * @brief get au2.
 */
static __attribute__((always_inline)) INLINE float
  part_get_au2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_au2;
}

/**
 * @brief get a pointer to au2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to au2. If you need read-only access to au2, use part_get_const_au2_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_au2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_au2;
}

/**
 * @brief get read-only access to pointer to au2.
 * If you need write access to au2, use part_get_au2_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_au2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_au2;
}

/**
 * @brief set the value of au2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_au2(struct part *restrict p, const float au2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_au2 = au2;
}


/**
 * @brief get bu2.
 */
static __attribute__((always_inline)) INLINE float
  part_get_bu2(const struct part *restrict p) {
#ifdef DEBUG
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_bu2;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to bu2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to bu2. If you need read-only access to bu2, use part_get_const_bu2_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_bu2_p(struct part *restrict p) {
#ifdef DEBUG
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_bu2;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to bu2.
 * If you need write access to bu2, use part_get_bu2_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_bu2_p(const struct part *restrict p) {
#ifdef DEBUG
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_bu2;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of bu2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_bu2(struct part *restrict p, const float bu2) {
#ifdef DEBUG
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_bu2 = bu2;
#endif
}


/**
 * @brief get cu2.
 */
static __attribute__((always_inline)) INLINE float
  part_get_cu2(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_cu2;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to cu2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to cu2. If you need read-only access to cu2, use part_get_const_cu2_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_cu2_p(struct part *restrict p) {
#ifdef NODEBUG
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_cu2;
#else
  return NULL;
#endif
}

/**
 * @brief get read-only access to pointer to cu2.
 * If you need write access to cu2, use part_get_cu2_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_cu2_p(const struct part *restrict p) {
#ifdef NODEBUG
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_cu2;
#else
  return NULL;
#endif
}

/**
 * @brief set the value of cu2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_cu2(struct part *restrict p, const float cu2) {
#ifdef NODEBUG
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_cu2 = cu2;
#endif
}




/**
 * @brief get gpu2.
 */
static __attribute__((always_inline)) INLINE struct gp*
  part_get_gpu2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_gpu2;
}

/**
 * @brief get a pointer to gpu2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to gpu2. If you need read-only access to gpu2, use part_get_const_gpu2_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gp**
  part_get_gpu2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_gpu2;
}/**
 * @brief set the value of gpu2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_gpu2(struct part *restrict p,  struct gp* gpu2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_gpu2 = gpu2;
}


/**
 * @brief get du2.
 */
static __attribute__((always_inline)) INLINE float
  part_get_du2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_ppu2._du2;
}

/**
 * @brief get a pointer to du2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to du2. If you need read-only access to du2, use part_get_const_du2_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_du2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_ppu2._du2;
}

/**
 * @brief get read-only access to pointer to du2.
 * If you need write access to du2, use part_get_du2_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_du2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_ppu2._du2;
}

/**
 * @brief set the value of du2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_du2(struct part *restrict p, const float du2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_ppu2._du2 = du2;
}


/**
 * @brief get eu2.
 */
static __attribute__((always_inline)) INLINE float
  part_get_eu2(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return part2_s->_ppu2._eu2;
}

/**
 * @brief get a pointer to eu2.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to eu2. If you need read-only access to eu2, use part_get_const_eu2_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_eu2_p(struct part *restrict p) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_ppu2._eu2;
}

/**
 * @brief get read-only access to pointer to eu2.
 * If you need write access to eu2, use part_get_eu2_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_eu2_p(const struct part *restrict p) {
  const struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  return &part2_s->_ppu2._eu2;
}

/**
 * @brief set the value of eu2.
 */
static __attribute__((always_inline)) INLINE void
  part_set_eu2(struct part *restrict p, const float eu2) {
  struct part2* part2_s = p->_cell_part_data->_part2 + p->_cell_offset;
  part2_s->_ppu2._eu2 = eu2;
}



#endif /* SWIFT_HYDRO_PART_TEST_H */
