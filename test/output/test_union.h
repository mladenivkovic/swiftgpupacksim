
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
  union {

    float _a;

#ifdef DEBUG
    float _b;
#endif

#ifdef NODEBUG
    float _c;
#endif

   };

  union {

    struct gp* _gp;

    struct {

      float _d;

      float _e;

     }_pp;

   };

};

/**
 * @brief get a.
 */
static __attribute__((always_inline)) INLINE float
  part_get_a(const struct part *restrict p) {
  return p->_a;
}

/**
 * @brief get a pointer to a.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to a. If you need read-only access to a, use part_get_const_a_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_a_p(struct part *restrict p) {
  return &p->_a;
}


/**
 * @brief get read-only access to pointer to a.
 * If you need write access to a, use part_get_a_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_a_p(const struct part *restrict p) {
  return &p->_a;
}


/**
 * @brief set the value of a.
 */
static __attribute__((always_inline)) INLINE void
  part_set_a(struct part *restrict p, const float a) {
  p->_a = a;
}


/**
 * @brief get b.
 */
static __attribute__((always_inline)) INLINE float
  part_get_b(const struct part *restrict p) {
#ifdef DEBUG
  return p->_b;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to b.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to b. If you need read-only access to b, use part_get_const_b_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_b_p(struct part *restrict p) {
#ifdef DEBUG
  return &p->_b;
#else
  return NULL;
#endif
}


/**
 * @brief get read-only access to pointer to b.
 * If you need write access to b, use part_get_b_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_b_p(const struct part *restrict p) {
#ifdef DEBUG
  return &p->_b;
#else
  return NULL;
#endif
}


/**
 * @brief set the value of b.
 */
static __attribute__((always_inline)) INLINE void
  part_set_b(struct part *restrict p, const float b) {
#ifdef DEBUG
  p->_b = b;
#endif
}


/**
 * @brief get c.
 */
static __attribute__((always_inline)) INLINE float
  part_get_c(const struct part *restrict p) {
#ifdef NODEBUG
  return p->_c;
#else
  return FLT_MAX;
#endif
}

/**
 * @brief get a pointer to c.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to c. If you need read-only access to c, use part_get_const_c_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_c_p(struct part *restrict p) {
#ifdef NODEBUG
  return &p->_c;
#else
  return NULL;
#endif
}


/**
 * @brief get read-only access to pointer to c.
 * If you need write access to c, use part_get_c_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_c_p(const struct part *restrict p) {
#ifdef NODEBUG
  return &p->_c;
#else
  return NULL;
#endif
}


/**
 * @brief set the value of c.
 */
static __attribute__((always_inline)) INLINE void
  part_set_c(struct part *restrict p, const float c) {
#ifdef NODEBUG
  p->_c = c;
#endif
}




/**
 * @brief get gp.
 */
static __attribute__((always_inline)) INLINE struct gp*
  part_get_gp(const struct part *restrict p) {
  return p->_gp;
}

/**
 * @brief get a pointer to gp.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to gp. If you need read-only access to gp, use part_get_const_gp_p() instead.
 */
static __attribute__((always_inline)) INLINE struct gp**
  part_get_gp_p(struct part *restrict p) {
  return &p->_gp;
}

/**
 * @brief set the value of gp.
 */
static __attribute__((always_inline)) INLINE void
  part_set_gp(struct part *restrict p,  struct gp* gp) {
  p->_gp = gp;
}


/**
 * @brief get d.
 */
static __attribute__((always_inline)) INLINE float
  part_get_d(const struct part *restrict p) {
  return p->_pp._d;
}

/**
 * @brief get a pointer to d.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to d. If you need read-only access to d, use part_get_const_d_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_d_p(struct part *restrict p) {
  return &p->_pp._d;
}


/**
 * @brief get read-only access to pointer to d.
 * If you need write access to d, use part_get_d_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_d_p(const struct part *restrict p) {
  return &p->_pp._d;
}


/**
 * @brief set the value of d.
 */
static __attribute__((always_inline)) INLINE void
  part_set_d(struct part *restrict p, const float d) {
  p->_pp._d = d;
}


/**
 * @brief get e.
 */
static __attribute__((always_inline)) INLINE float
  part_get_e(const struct part *restrict p) {
  return p->_pp._e;
}

/**
 * @brief get a pointer to e.
 * Use this only if you need to modify the value, i.e. if you need write access
 * to e. If you need read-only access to e, use part_get_const_e_p() instead.
 */
static __attribute__((always_inline)) INLINE float*
  part_get_e_p(struct part *restrict p) {
  return &p->_pp._e;
}


/**
 * @brief get read-only access to pointer to e.
 * If you need write access to e, use part_get_e_p() instead.
 */
static __attribute__((always_inline)) INLINE const float*
  part_get_const_e_p(const struct part *restrict p) {
  return &p->_pp._e;
}


/**
 * @brief set the value of e.
 */
static __attribute__((always_inline)) INLINE void
  part_set_e(struct part *restrict p, const float e) {
  p->_pp._e = e;
}




#endif /* SWIFT_HYDRO_PART_TEST_H */
