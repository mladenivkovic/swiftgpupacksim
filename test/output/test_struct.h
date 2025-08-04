
#ifndef SWIFT_HYDRO_PART_TEST_H
#define SWIFT_HYDRO_PART_TEST_H

#include <float.h>
#include <limits.h>
#include <stddef.h>

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

      struct gpart* _gp;

      timebin_t _t;

     }_nested1;

   }_st2;

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
 * @brief get b,
 * for read and write access. For read-only access, use
 * part_get_const_b() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_b(struct part *restrict p) {
  return p->_st1._b;
}

/**
 * @brief get b,
 * for read-only access.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_b(const struct part *restrict p) {
  return p->_st1._b;
}

/**
 * @brief get b,
 * by index.
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
 * @brief set the value of b, by index.
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
 * @brief get gp.
 */
static __attribute__((always_inline)) INLINE struct gpart*
  part_get_gp(const struct part *restrict p) {
  return p->_st2._nested1._gp;
}

/**
 * @brief get a pointer to gp.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to gp. If you need read-only access to gp, use part_get_const_gp_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gpart**
  part_get_gp_p(struct part *restrict p) {
  return &p->_st2._nested1._gp;
}

/**
 * @brief set the value of gp.
 */
static __attribute__((always_inline)) INLINE void
  part_set_gp(struct part *restrict p,  struct gpart* gp) {
  p->_st2._nested1._gp = gp;
}


/**
 * @brief get t.
 */
static __attribute__((always_inline)) INLINE timebin_t
  part_get_t(const struct part *restrict p) {
  return p->_st2._nested1._t;
}

/**
 * @brief get a pointer to t.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to t. If you need read-only access to t, use part_get_const_t_p() instead.
 */
static __attribute__((always_inline)) INLINE timebin_t*
  part_get_t_p(struct part *restrict p) {
  return &p->_st2._nested1._t;
}


/**
 * @brief get read-only access to pointer to t.
 * If you need write access to t, use part_get_t_p() instead.
 */
static __attribute__((always_inline)) INLINE const timebin_t*
  part_get_const_t_p(const struct part *restrict p) {
  return &p->_st2._nested1._t;
}


/**
 * @brief set the value of t.
 */
static __attribute__((always_inline)) INLINE void
  part_set_t(struct part *restrict p, const timebin_t t) {
  p->_st2._nested1._t = t;
}




#endif /* SWIFT_HYDRO_PART_TEST_H */
