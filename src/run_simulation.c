#include "run_simulation.h"

#include "cuda/gpu_data_buffers.h"
#include "cuda/gpu_packing_defines.h"
#include "io.h"
#include "parts.h"
#include "runner_doiact_functions_hydro_gpu.h"

#include <math.h>


/**
 * Dummy function for now.
 *
 * @param data: Struct holding pointers to all particle data
 * @param ci_offset: index at which this cell's particles start in global
 * particle array
 * @param count: nr of particles in this cell
 * @param nr_parts: total number of particles in this simulation
 */
void pack_cell_dummy(struct part_arrays *data, size_t ci_offset, size_t count) {

#ifdef SWIFT_DEBUG_CHECKS
  if ((ci_offset + count) > data->nr_parts)
    error("Will reach beyond array length: %lu %lu %lu %lu", ci_offset, count,
          ci_offset + count, data->nr_parts);
#endif

  for (size_t i = ci_offset; i < ci_offset + count; i++) {
    struct part *p = &data->p[i];
    part_set_id(p, part_get_id(p) + 1);
  }
}


/**
 * Perform the actual work of a single event.
 *
 * This region may be executed in parallel, so use atomics where
 * necessary.
 *
 * @param event: Recorded/logged event in measurement run to be reproduced in
 * this function
 * @param data: Struct holding pointers to all particle data
 * @param timings_log: Array to store measured times (already converted from
 * ticks during measurement run)
 * @param timers: Array to store timers (ticks) for this step of the simulation
 */
void replay_event(const struct packing_data *event, struct part_arrays *data,
                  struct runner *r, struct cell *ci, struct cell *cj,
                  struct task *t, struct scheduler *sched,
                  struct pack_vars_self *pack_vars_self_dens,
                  struct pack_vars_pair *pack_vars_pair_dens,
                  struct pack_vars_self *pack_vars_self_grad,
                  struct pack_vars_pair *pack_vars_pair_grad,
                  struct pack_vars_self *pack_vars_self_forc,
                  struct pack_vars_pair *pack_vars_pair_forc,
                  int2 *task_first_part_f4, int2 *task_first_part_f4_g,
                  int2 *task_first_part_f4_f,
                  struct part_aos_f4_send *parts_aos_f4_send,
                  struct part_aos_f4_recv *parts_aos_f4_recv,
                  struct part_aos_f4_g_send *parts_aos_grad_f4_send,
                  struct part_aos_f4_g_recv *parts_aos_grad_f4_recv,
                  struct part_aos_f4_f_send *parts_aos_forc_f4_send,
                  struct part_aos_f4_f_recv *parts_aos_forc_f4_recv,
                  struct part_aos_f4_send *parts_aos_pair_f4_send,
                  struct part_aos_f4_recv *parts_aos_pair_f4_recv,
                  struct part_aos_f4_g_send *parts_aos_pair_f4_g_send,
                  struct part_aos_f4_g_recv *parts_aos_pair_f4_g_recv,
                  struct part_aos_f4_f_send *parts_aos_pair_f4_f_send,
                  struct part_aos_f4_f_recv *parts_aos_pair_f4_f_recv,
                  ticks timers_step[timer_count],
                  double timings_log_step[timer_count]) {

  enum task_types type = event->task_type;
  if (type == task_type_density_self) {
    TIMER_TIC;

    runner_doself1_pack_f4(r, sched, pack_vars_self_dens, ci, t,
                           parts_aos_f4_send, task_first_part_f4);

    TIMER_TOC_LOCATION(timer_density_self, timers_step);
    atomic_add_d(&timings_log_step[timer_density_self], event->timing);
  } else if (type == task_type_gradient_self) {
    TIMER_TIC;
    pack_cell_dummy(data, event->ci_offset, event->ci_count);
    TIMER_TOC_LOCATION(timer_gradient_self, timers_step);
    atomic_add_d(&timings_log_step[timer_gradient_self], event->timing);
  } else if (type == task_type_force_self) {
    TIMER_TIC;
    pack_cell_dummy(data, event->ci_offset, event->ci_count);
    TIMER_TOC_LOCATION(timer_force_self, timers_step);
    atomic_add_d(&timings_log_step[timer_force_self], event->timing);
  } else if (type == task_type_density_pair) {
    TIMER_TIC;
    pack_cell_dummy(data, event->ci_offset, event->ci_count);
    pack_cell_dummy(data, event->cj_offset, event->cj_count);
    TIMER_TOC_LOCATION(timer_density_pair, timers_step);
    atomic_add_d(&timings_log_step[timer_density_pair], event->timing);
  } else if (type == task_type_gradient_pair) {
    TIMER_TIC;
    pack_cell_dummy(data, event->ci_offset, event->ci_count);
    pack_cell_dummy(data, event->cj_offset, event->cj_count);
    TIMER_TOC_LOCATION(timer_gradient_pair, timers_step);
    atomic_add_d(&timings_log_step[timer_gradient_pair], event->timing);
  } else if (type == task_type_force_pair) {
    TIMER_TIC;
    pack_cell_dummy(data, event->ci_offset, event->ci_count);
    pack_cell_dummy(data, event->cj_offset, event->cj_count);
    TIMER_TOC_LOCATION(timer_force_pair, timers_step);
    atomic_add_d(&timings_log_step[timer_force_pair], event->timing);
  } else {
    error("Unknown task type");
  }
}


/**
 * Run the actual thing.
 */
void run_simulation(struct parameters *params) {

  /* Allocate data to work on */
  struct part_arrays part_data;
  init_parts(&part_data, params);

  message("Starting simulation.");

  /* Init timers */
  ticks timers_step[timer_count];
  timers_reset(timers_step);
  ticks timers_full[timer_count];
  timers_reset(timers_full);
  double timing_log_step[timer_count];
  timing_reset_time(timing_log_step);
  double timing_log_full[timer_count];
  timing_reset_time(timing_log_full);


  // - init parallel region here
  // - check that we're running on the same nr of threads as
  //   the original simulation

  // For each thread:
  int thread_id = 0;

  /* Pretend we're running swift.*/
  struct runner r;
  r.id = thread_id;

  struct scheduler sched;

  struct pack_vars_self *pack_vars_self_dens = NULL;
  struct pack_vars_pair *pack_vars_pair_dens = NULL;
  struct pack_vars_self *pack_vars_self_grad = NULL;
  struct pack_vars_pair *pack_vars_pair_grad = NULL;
  struct pack_vars_self *pack_vars_self_forc = NULL;
  struct pack_vars_pair *pack_vars_pair_forc = NULL;
  gpu_data_init_pack_arrays(&pack_vars_self_dens, &pack_vars_pair_dens,
                            &pack_vars_self_grad, &pack_vars_pair_grad,
                            &pack_vars_self_forc, &pack_vars_pair_forc,
                            /* params->verbose */
                            1);

#ifdef SWIFT_DEBUG_CHECKS
  swift_assert(pack_vars_self_dens != NULL);
  swift_assert(pack_vars_pair_dens != NULL);
  swift_assert(pack_vars_self_grad != NULL);
  swift_assert(pack_vars_pair_grad != NULL);
  swift_assert(pack_vars_self_forc != NULL);
  swift_assert(pack_vars_pair_forc != NULL);
#endif

  // Keep track of first and last particles for each task (particle data is
  // arranged in long arrays containing particles from all the tasks we will
  // work with)
  /* A. N.: Needed for offloading self tasks as we use these to sort through
   *        which parts need to interact with which */
  int2 *task_first_part_f4 = NULL;
  int2 *task_first_part_f4_g = NULL;
  int2 *task_first_part_f4_f = NULL;
  /* int2 *d_task_first_part_f4; */
  /* int2 *d_task_first_part_f4_f; */
  /* int2 *d_task_first_part_f4_g; */

  gpu_data_init_first_part_host_arrays(
      &task_first_part_f4, &task_first_part_f4_g, &task_first_part_f4_f,
      /* &d_task_first_part_f4, */
      /* &d_task_first_part_f4_g, */
      /* &d_task_first_part_f4_f */
      /* params->verbose */
      1);

#ifdef SWIFT_DEBUG_CHECKS
  swift_assert(task_first_part_f4 != NULL);
  swift_assert(task_first_part_f4_g != NULL);
  swift_assert(task_first_part_f4_f != NULL);
#endif

  const int target_n_tasks = TARGET_N_TASKS_PACK_SIZE;
  const int np_per_cell = PARTS_PER_CELL;
  const int buff = ceil(0.5 * np_per_cell);
  const int count_max_parts_tmp =
      64 * 8 * target_n_tasks * (np_per_cell + buff);

  struct part_aos_f4_send *parts_aos_f4_send = NULL;
  struct part_aos_f4_recv *parts_aos_f4_recv = NULL;
  struct part_aos_f4_f_send *parts_aos_forc_f4_send = NULL;
  struct part_aos_f4_f_recv *parts_aos_forc_f4_recv = NULL;
  struct part_aos_f4_g_send *parts_aos_grad_f4_send = NULL;
  struct part_aos_f4_g_recv *parts_aos_grad_f4_recv = NULL;
  struct part_aos_f4_send *parts_aos_pair_f4_send = NULL;
  struct part_aos_f4_recv *parts_aos_pair_f4_recv = NULL;
  struct part_aos_f4_f_send *parts_aos_pair_f4_f_send = NULL;
  struct part_aos_f4_f_recv *parts_aos_pair_f4_f_recv = NULL;
  struct part_aos_f4_g_send *parts_aos_pair_f4_g_send = NULL;
  struct part_aos_f4_g_recv *parts_aos_pair_f4_g_recv = NULL;

  gpu_data_init_send_recv_host_arrays(
      &parts_aos_f4_send, &parts_aos_f4_recv, &parts_aos_grad_f4_send,
      &parts_aos_grad_f4_recv, &parts_aos_forc_f4_send, &parts_aos_forc_f4_recv,
      &parts_aos_pair_f4_send, &parts_aos_pair_f4_recv,
      &parts_aos_pair_f4_g_send, &parts_aos_pair_f4_g_recv,
      &parts_aos_pair_f4_f_send, &parts_aos_pair_f4_f_recv, count_max_parts_tmp,
      /* params->verbose */
      1);

#ifdef SWIFT_DEBUG_CHECKS
  swift_assert(parts_aos_f4_send != NULL);
  swift_assert(parts_aos_f4_recv != NULL);
  swift_assert(parts_aos_grad_f4_send != NULL);
  swift_assert(parts_aos_grad_f4_recv != NULL);
  swift_assert(parts_aos_forc_f4_send != NULL);
  swift_assert(parts_aos_forc_f4_recv != NULL);
  swift_assert(parts_aos_pair_f4_send != NULL);
  swift_assert(parts_aos_pair_f4_recv != NULL);
  swift_assert(parts_aos_pair_f4_g_send != NULL);
  swift_assert(parts_aos_pair_f4_g_recv != NULL);
  swift_assert(parts_aos_pair_f4_f_send != NULL);
  swift_assert(parts_aos_pair_f4_f_recv != NULL);
#endif


  /* -------------------------------------------------*/
  /* Loop over recorded simulation steps              */
  /* this loop corresponds to the 'Main loop', i.e.   */
  /* the first `while (1){}` in runner_main           */
  /* -------------------------------------------------*/
  for (int step = 0; step < params->nr_steps; step++) {

    // Initialise packing counters
    pack_vars_self_dens->tasks_packed = 0;
    pack_vars_pair_dens->tasks_packed = 0;
    pack_vars_self_dens->count_parts = 0;
    pack_vars_pair_dens->count_parts = 0;
    pack_vars_pair_dens->task_locked = 0;
    pack_vars_pair_dens->top_tasks_packed = 0;
    // Initialise packing counters
    pack_vars_self_forc->tasks_packed = 0;
    pack_vars_pair_forc->tasks_packed = 0;
    pack_vars_self_forc->count_parts = 0;
    pack_vars_pair_forc->count_parts = 0;
    // Initialise packing counters
    pack_vars_self_grad->tasks_packed = 0;
    pack_vars_pair_grad->tasks_packed = 0;
    pack_vars_self_grad->count_parts = 0;
    pack_vars_pair_grad->count_parts = 0;

    for (int i = 0; i < target_n_tasks; i++)
      pack_vars_pair_dens->leaf_list[i].n_leaves = 0;

    /* Get file to read */
    char logfile[IO_MAX_FILENAME_SIZE];
    io_util_construct_log_filename(logfile, 0, step, params);

    /* Read trace */
    struct packing_data *packing_sequence = NULL;
    size_t n_events = 0;
    io_read_logged_events_file(logfile, &packing_sequence, &n_events, params);

    if (params->verbose)
      message("Thread %d step %d found %lu events.", thread_id, step, n_events);


    /* ----------------------------------------------*/
    /* Loop over recorded packing tasks.             */
    /* This loop corresponds to second `while (1){}` */
    /* loop in runner_main                           */
    /* ----------------------------------------------*/
    for (size_t i = 0; i < n_events; i++) {
      struct packing_data event = packing_sequence[i];

      /* Get cells and feed them into the task struct*/
      struct cell ci;
      init_cell(&ci);
      ci.hydro.count = event.ci_count;
      ci.hydro.parts = part_data.p + event.ci_offset;
#ifdef SWIFT_DEBUG_CHECKS
      if ((event.ci_offset + event.ci_count) > part_data.nr_parts)
        error("Will reach beyond array length: %lu %d %lu %lu", event.ci_offset,
              event.ci_count, event.ci_offset + event.ci_count,
              part_data.nr_parts);
#endif
      struct task t;
      t.ci = &ci;
      t.cj = NULL;

      struct cell cj;
      init_cell(&cj);
      if (event.task_type == task_type_density_pair ||
          event.task_type == task_type_gradient_pair ||
          event.task_type == task_type_force_pair) {
        cj.hydro.count = event.cj_count;
        cj.hydro.parts = part_data.p + event.cj_offset;

#ifdef SWIFT_DEBUG_CHECKS
        if ((event.cj_offset + event.cj_count) > part_data.nr_parts)
          error("Will reach beyond array length: %lu %d %lu %lu",
                event.cj_offset, event.cj_count,
                event.cj_offset + event.cj_count, part_data.nr_parts);
#endif
        t.cj = &cj;
      }

      replay_event(&event, &part_data, &r, &ci, &cj, &t, &sched,
                   pack_vars_self_dens, pack_vars_pair_dens,
                   pack_vars_self_grad, pack_vars_pair_grad,
                   pack_vars_self_forc, pack_vars_pair_forc, task_first_part_f4,
                   task_first_part_f4_g, task_first_part_f4_f,
                   parts_aos_f4_send, parts_aos_f4_recv, parts_aos_grad_f4_send,
                   parts_aos_grad_f4_recv, parts_aos_forc_f4_send,
                   parts_aos_forc_f4_recv, parts_aos_pair_f4_send,
                   parts_aos_pair_f4_recv, parts_aos_pair_f4_g_send,
                   parts_aos_pair_f4_g_recv, parts_aos_pair_f4_f_send,
                   parts_aos_pair_f4_f_recv, timers_step, timing_log_step);
    }

    message("Finished step %d", step);
    if (params->print_each_step) print_timers(timers_step, timing_log_step);
    free(packing_sequence);

    // omp single
    for (int i = 0; i < timer_count; i++) {
      timers_full[i] += timers_step[i];
      timers_step[i] = 0;
      timing_log_full[i] += timing_log_step[i];
      timing_log_step[i] = 0.;
    }
    // omp barrier
  }

  int *testvar = NULL;
  test_alloc_and_free(&testvar);

  gpu_data_clear_pack_arrays(&pack_vars_self_dens, &pack_vars_pair_dens,
                             &pack_vars_self_grad, &pack_vars_pair_grad,
                             &pack_vars_self_forc, &pack_vars_pair_forc);

  gpu_data_clear_first_part_host_arrays(
      &task_first_part_f4, &task_first_part_f4_g, &task_first_part_f4_f
      /* &d_task_first_part_f4, */
      /* &d_task_first_part_f4_g, */
      /* &d_task_first_part_f4_f */
  );

  gpu_data_clear_send_recv_host_arrays(
      &parts_aos_f4_send, &parts_aos_f4_recv, &parts_aos_grad_f4_send,
      &parts_aos_grad_f4_recv, &parts_aos_forc_f4_send, &parts_aos_forc_f4_recv,
      &parts_aos_pair_f4_send, &parts_aos_pair_f4_recv,
      &parts_aos_pair_f4_g_send, &parts_aos_pair_f4_g_recv,
      &parts_aos_pair_f4_f_send, &parts_aos_pair_f4_f_recv);


  message("Finished simulation.");
  print_timers(timers_full, timing_log_full);

  clear_parts(&part_data);
}
