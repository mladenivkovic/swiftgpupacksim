#pragma once

/**
 * @brief Cell within the tree structure.
 *
 * Contains particles, links to tasks, a multipole object and counters.
 */
#include "../config.h"

#include <stdint.h>

/* Local includes. */
#include "cell_hydro.h"
#include "swift_placeholders/cell_other_physics.h"
#include "swift_placeholders/task.h"

struct cell {

  /*! The cell location on the grid (corner nearest to the origin). */
  double loc[3];

  /*! The cell dimensions. */
  double width[3];

  /*! Pointers to the next level of cells. */
  struct cell *progeny[8];

  union {

    /*! Linking pointer for "memory management". */
    struct cell *next;

    /*! Parent cell. */
    struct cell *parent;
  };

  /*! Pointer to the top-level cell in a hierarchy */
  struct cell *top;

  /*! Super cell, i.e. the highest-level parent cell with *any* task */
  struct cell *super;

  /*! Cell flags bit-mask. */
  volatile uint32_t flags;

  /*! Hydro variables */
  struct cell_hydro hydro;

  /*! Grav variables */
  struct cell_grav grav;

  /*! Stars variables */
  struct cell_stars stars;

  /*! Black hole variables */
  struct cell_black_holes black_holes;

  /*! Sink particles variables */
  struct cell_sinks sinks;

  /*! The grid variables */
  struct cell_grid grid;

  /*! Radiative transfer variables */
  struct cell_rt rt;

#ifdef WITH_MPI
  /*! MPI variables */
  struct {

    union {
      /* Single list of all send tasks associated with this cell. */
      struct link *send;

      /* Single list of all recv tasks associated with this cell. */
      struct link *recv;
    };

    union {
      /* Single list of all pack tasks associated with this cell. */
      struct link *pack;

      /* Single list of all unpack tasks associated with this cell. */
      struct link *unpack;
    };

    /*! Bit mask of the proxies this cell is registered with. */
    unsigned long long int sendto;

    /*! Pointer to this cell's packed representation. */
    struct pcell *pcell;

    /*! Size of the packed representation */
    int pcell_size;

    /*! MPI tag associated with this cell */
    int tag;

  } mpi;
#endif

  /*! The first kick task */
  struct task *kick1;

  /*! The second kick task */
  struct task *kick2;

  /*! The task to compute time-steps */
  struct task *timestep;

  /*! The task to limit the time-step of inactive particles */
  struct task *timestep_limiter;

  /*! The task to synchronize the time-step of inactive particles hit by
   * feedback */
  struct task *timestep_sync;

  /*! The task to recursively collect time-steps */
  struct task *timestep_collect;

#ifdef WITH_CSDS
  /*! The csds task */
  struct task *csds;
#endif

  /*! Minimum dimension, i.e. smallest edge of this cell (min(width)). */
  float dmin;

  /*! When walking the tree and running loops at different level, this is
   * the minimal h that can be processed at this level */
  float h_min_allowed;

  /*! When walking the tree and running loops at different level, this is
   * the maximal h that can be processed at this level */
  float h_max_allowed;

  /*! ID of the previous owner, e.g. runner. */
  short int owner;

  /*! ID of a threadpool thread that maybe associated with this cell. */
  short int tpid;

  /*! ID of the node this cell lives on. */
  int nodeID;

  /*! The depth of this cell in the tree. */
  char depth;

  /*! Is this cell split ? */
  char split;

  /*! The maximal depth of this cell and its progenies */
  char maxdepth;

#if defined(SWIFT_DEBUG_CHECKS) || defined(SWIFT_CELL_GRAPH)
  /* Cell ID (for debugging) */
  long long cellID;
#endif

#ifdef SWIFT_DEBUG_CHECKS

  /*! Number of tasks that are associated with this cell. */
  short int nr_tasks;

  /*! The list of tasks that have been executed on this cell */
  char tasks_executed[task_type_count];

  /*! The list of sub-tasks that have been executed on this cell */
  char subtasks_executed[task_type_count];
#endif

  /* struct ghost_stats ghost_statistics; */

} SWIFT_STRUCT_ALIGN;


/* Get hydro particle array */
static __attribute__((always_inline)) INLINE struct part *cell_get_hydro_parts(
    struct cell *restrict c) {
  return c->hydro.part_arrs._part;
}

/* Get const hydro particle array */
static __attribute__((always_inline)) INLINE const struct part *
cell_get_const_hydro_parts(const struct cell *restrict c) {
  return c->hydro.part_arrs._part;
}

/**
 * Minimalistic cell initialisation for this benchmark
 */
static __attribute__((always_inline)) INLINE void init_cell(
    struct cell *c, int count, const struct hydro_part_arrays *all_parts,
    int offset, omp_lock_t *part_locks) {

  c->loc[0] = 1.;
  c->loc[1] = 1.;
  c->loc[2] = 1.;

  c->hydro.count = count;
  part_arrays_set_pointer_offset(&c->hydro.part_arrs, all_parts, offset);

  /* Before we write into particles, lock them. */
  for (int i = 0; i < count; i++) {
    omp_set_lock(&part_locks[offset + i]);
  }

#if defined(SPHENIX_AOS_PARTICLE)
#elif defined(SPHENIX_SOA_PARTICLE)
  for (int i = 0; i < count; i++) {
    c->hydro.part_arrs._part[i]._cell_offset = i;
    c->hydro.part_arrs._part[i]._cell_part_arrays = &c->hydro.part_arrs;
  }
#else
#pragma error "particle handling for this memory layout not implemented"
#endif
}

/**
 * Clean up after yourself. In particular, release the locks.
 */
static __attribute__((always_inline)) INLINE void destroy_cell(
    struct cell *c, int count, int offset, omp_lock_t *part_locks) {

  for (int i = 0; i < count; i++) {
    omp_unset_lock(&part_locks[offset + i]);
  }
}
