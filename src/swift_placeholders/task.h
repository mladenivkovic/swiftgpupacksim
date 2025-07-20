/**
 * Task data.
 */

#pragma once

#include "align.h"
#include "cycle.h"
#include "timeline.h"

enum task_types {
  task_type_none = 0,
  task_type_density_self,
  task_type_density_pair,
  task_type_gradient_self,
  task_type_gradient_pair,
  task_type_force_self,
  task_type_force_pair,
  task_type_count
};

enum task_subtypes {
  task_subtype_none = 0,
  task_subtype_we_dont_use_these_here,
  task_subtype_count
};


extern const char *task_type_names[task_type_count];


struct task {

  /*! Pointers to the cells this task acts upon */
  struct cell *ci, *cj;

  int done;  // A. Nasar

  int gpu_done;

  int corner_pair;

  /*! Pointers to the cells this task acts upon */
  struct cell **ci_unpack;  //, **cj;

  /*! List of tasks unlocked by this one */
  struct task **unlock_tasks;

  /*! Flags used to carry additional information (e.g. sort directions) */
  long long flags;

#ifdef WITH_MPI

  /*! Buffer for this task's communications */
  void *buff;

  /*! MPI request corresponding to this task */
  MPI_Request req;

#endif

  /*! Rank of a task in the order */
  int rank;

  /*! Weight of the task */
  float weight;

  /*! Number of tasks unlocked by this one */
  int nr_unlock_tasks;

  /*! Number of unsatisfied dependencies */
  int wait;

  /*! Type of the task */
  enum task_types type;

  /*! Sub-type of the task (for the tasks that have one */
  enum task_subtypes subtype;

  /*! Should the scheduler skip this task ? */
  char skip;

  /*! Is this task implicit (i.e. does not do anything) ? */
  char implicit;

#ifdef SWIFT_DEBUG_TASKS
  /*! ID of the queue or runner owning this task */
  short int rid;

  /*! Information about the direction of the pair task */
  short int sid;
#endif

  /*! Start and end time of this task */
  ticks tic, toc;

  ticks total_cpu_pack_ticks;
  ticks total_cpu_unpack_ticks;

  /* Total time spent running this task */
  ticks total_ticks;

#ifdef SWIFT_DEBUG_CHECKS
  /* When was this task last run? */
  integertime_t ti_run;
#endif /* SWIFT_DEBUG_CHECKS */

} SWIFT_STRUCT_ALIGN;


