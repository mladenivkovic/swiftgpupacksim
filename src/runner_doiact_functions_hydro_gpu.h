/* #include "runner_doiact_hydro.h" */
#include "cuda/pack_vars.h"
#include "cuda/part_gpu.h"
#include "runner.h"
#include "scheduler.h"
#include "task.h"

/* #include <atomic.h> */
#include "runner_gpu_pack_functions.h"


double runner_doself1_pack_f4(struct runner *r, struct scheduler *s,
                              struct pack_vars_self *pack_vars, struct cell *ci,
                              struct task *t,
                              struct part_aos_f4_send *parts_send,
                              int2 *task_first_part_f4) {

  /* Timers for how long this all takes.
   * t0 and t1 are from start to finish including GPU calcs
   * tp0 and tp1 only time packing and unpacking*/
  /* struct timespec t0, t1;  // */
  /* clock_gettime(CLOCK_REALTIME, &t0); */
  /* Find my queue for use later*/
  /* int qid = r->qid; */

  /*Place pointers to the task and cells packed in an array for use later
   * when unpacking after the GPU offload*/
  int tasks_packed = pack_vars->tasks_packed;
  pack_vars->task_list[tasks_packed] = t;
  pack_vars->cell_list[tasks_packed] = ci;

  /* Identify row in particle arrays where this task starts*/
  task_first_part_f4[tasks_packed].x = pack_vars->count_parts;
  int *count_parts_self = &pack_vars->count_parts;

  /* This re-arranges the particle data from cell->hydro->parts into a
  long array of part structs*/
  runner_doself1_gpu_pack_neat_aos_f4(
      r, ci, parts_send, 0 /*timer. 0 no timing, 1 for timing*/,
      count_parts_self, tasks_packed, pack_vars->count_max_parts);

  /* Identify the row in the array where this task ends (row id of its
     last particle)*/
  task_first_part_f4[tasks_packed].y = pack_vars->count_parts;

  /* Identify first particle for each bundle of tasks */
  const int bundle_size = pack_vars->bundle_size;
  if (tasks_packed % bundle_size == 0) {
    int bid = tasks_packed / bundle_size;
    pack_vars->bundle_first_part[bid] = task_first_part_f4[tasks_packed].x;
    pack_vars->bundle_first_task_list[bid] = tasks_packed;
  }

  /* Tell the cell it has been packed */
  /* ci->pack_done++; */

  /* Record that we have now done a packing (self) */
  t->done = 1;
  pack_vars->tasks_packed++;
  pack_vars->launch = 0;
  pack_vars->launch_leftovers = 0;

  /*Get a lock to the queue so we can safely decrement counter and check for
   * launch leftover condition*/
  /* lock_lock(&s->queues[qid].lock); */
  /* s->queues[qid].n_packs_self_left_d--; */

  /* if (s->queues[qid].n_packs_self_left_d < 1) pack_vars->launch_leftovers =
   * 1; */
  /* lock_unlock(&s->queues[qid].lock); */

  /* Have we packed enough tasks to offload to GPU? */
  if (pack_vars->tasks_packed == pack_vars->target_n_tasks)
    pack_vars->launch = 1;

  /* Record the end of packing time */
  /* clock_gettime(CLOCK_REALTIME, &t1); */
  /* Release the lock on the cell */
  /* cell_unlocktree(ci); */
  t->gpu_done = 1;
  /*Calculate time spent packing and return to runner_main*/
  /* return (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1000000000.0;
   */
  return 0.;
}
