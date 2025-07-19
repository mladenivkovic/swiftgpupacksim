/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2013 Pedro Gonnet (pedro.gonnet@durham.ac.uk)
 *                    Matthieu Schaller (schaller@strw.leidenuniv.nl)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#ifndef SWIFT_SCHEDULER_H
#define SWIFT_SCHEDULER_H

/* Config parameters. */
#include <config.h>

/* MPI headers. */
#ifdef WITH_MPI
#include <mpi.h>
#endif

/* Some standard headers. */
#include <pthread.h>

/* Includes. */
/* #include "inline.h" */
/* #include "lock.h" */
#include "queue.h"
#include "task.h"
/* #include "threadpool.h" */

/* Some constants. */
#define scheduler_maxwait 3
#define scheduler_init_nr_unlocks 10000
#define scheduler_dosub 1
#define scheduler_maxsteal 10
#define scheduler_maxtries 2
#define scheduler_doforcesplit            \
  0 /* Beware: switching this on can/will \
       break engine_addlink as it assumes \
       a maximum number of tasks per cell. */

/* Flags . */
#define scheduler_flag_none 0
#define scheduler_flag_steal (1 << 1)

/* Data of a scheduler. */
struct scheduler {

  int nr_packs_self_dens_done;  // A. Nasar
  int nr_packs_pair_dens_done;
  int nr_packs_self_forc_done;
  int nr_packs_pair_forc_done;
  int nr_packs_self_grad_done;
  int nr_packs_pair_grad_done;

  volatile int *s_d_left;
  volatile int *s_g_left;
  volatile int *s_f_left;
  volatile int *p_d_left;
  volatile int *p_g_left;
  volatile int *p_f_left;
  /* Actual number of GPU tasks. */
  int nr_gpu_tasks;
  /* Number of tasks we want*/
  int target_gpu_tasks;
  /* Actual number of density pack tasks. */
  int nr_self_pack_tasks_d, nr_pair_pack_tasks_d;
  /* Actual number of force pack tasks. */
  int nr_self_pack_tasks_f, nr_pair_pack_tasks_f;
  /* Actual number of gradient pack tasks. */
  int nr_self_pack_tasks_g, nr_pair_pack_tasks_g;

  /*how many tasks we want to try and work on at once on the GPU*/
  int pack_size;
  int pack_size_pair;

  /* Scheduler flags. */
  unsigned int flags;

  /* Number of queues in this scheduler. */
  int nr_queues;

  /* Array of queues. */
  struct queue *queues;

  /* Total number of tasks. */
  int nr_tasks, size, tasks_next;

  /* Total number of waiting tasks. */
  int waiting;

  /* The task array. */
  struct task *tasks;

  /* The task indices. */
  int *tasks_ind;

  /* List of initial tasks. */
  int *tid_active;
  int active_count;

  /* The task unlocks. */
  struct task **volatile unlocks;
  int *volatile unlock_ind;
  volatile int nr_unlocks, size_unlocks, completed_unlock_writes;

  /* Lock for this scheduler. */
  swift_lock_type lock;

  /* Waiting queue. */
  pthread_mutex_t sleep_mutex;
  pthread_cond_t sleep_cond;

  /* The space associated with this scheduler. */
  struct space *space;

  /* Threadpool to use internally for mundane parallel work. */
  struct threadpool *threadpool;

  /* The node we are working on. */
  int nodeID;

  /* Maximum size of task messages, in bytes, to sent using non-buffered
   * MPI. */
  size_t mpi_message_limit;

  /* Total ticks spent running the tasks */
  ticks total_ticks;

  struct {
    /* Total ticks spent waiting for runners to come home. */
    ticks waiting_ticks;

    /* Total ticks spent by runners running tasks. */
    ticks active_ticks;
  } deadtime;

  /* Frequency of the dependency graph dumping. */
  int frequency_dependency;

  /* Specific cell to dump dependency graph for */
  long long dependency_graph_cellID;

  /* Frequency of the task levels dumping. */
  int frequency_task_levels;

#if defined(SWIFT_DEBUG_CHECKS)
  /* Stuff for the deadlock detector */

  /* How long to wait (in ms) before assuming we're in a deadlock */
  float deadlock_waiting_time_ms;

  /* Time at which last task was successfully retrieved from a queue */
  ticks last_successful_task_fetch;

  /* needed to dump queues on deadlock detection */
  struct engine *e;

#endif /* SWIFT_DEBUG_CHECKS */
};


#endif /* SWIFT_SCHEDULER_H */
