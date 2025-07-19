#pragma once

/**
 * @brief Cell within the tree structure.
 *
 * Contains particles, links to tasks, a multipole object and counters.
 */
#include "config.h"

#include <stdint.h>

/* Local includes. */
#include "cell_hydro.h"
#include "cell_other_physics.h"
#include "task.h"

struct cell {

  /*Marks a cell for GPU execution A. Nasar */

  int unpacker_cell;

  /*Marks a cell as having done its pack task 0->not 1-> yes*/
  int pack_done;
  /*Marks a cell as having done its pack task 0->not 1-> yes*/
  int pack_done_g;
  /*Marks a cell as having done its pack task 0->not 1-> yes*/
  int pack_done_f;

  /*Has the task run on the GPU? 0->No, 1-> Yes*/
  int gpu_done;
  /*Has the task run on the GPU? 0->No, 1-> Yes*/
  int gpu_done_g;
  /*Has the task run on the GPU? 0->No, 1-> Yes*/
  int gpu_done_f;

  /*Has the task run on the GPU? 0->No, 1-> Yes*/
  int unpack_done;
  /*Has the task run on the GPU? 0->No, 1-> Yes*/
  int unpack_done_g;
  /*Has the task run on the GPU? 0->No, 1-> Yes*/
  int unpack_done_f;

  /*Has the pair task run on the GPU? 0->No, 1-> Yes*/
  int gpu_done_pair;
  /*Has the pair task run on the GPU? 0->No, 1-> Yes*/
  int gpu_done_pair_g;
  /*Has the pair task run on the GPU? 0->No, 1-> Yes*/
  int gpu_done_pair_f;

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



/**
 * Minimalistic cell initialisation for this benchmark
 */
inline void init_cell(struct cell* c){

  c->loc[0] = 1.;
  c->loc[1] = 1.;
  c->loc[2] = 1.;

  c->hydro.count = 0;
  c->hydro.parts = 0;
}

