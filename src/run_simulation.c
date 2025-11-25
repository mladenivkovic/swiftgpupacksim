#include "run_simulation.h"

#include "io.h"
#include "parts.h"
#include "swift_placeholders/cuda/gpu_offload_data.h"
#include "swift_placeholders/cuda/gpu_part_pack_functions.h"
#include "swift_placeholders/engine.h"
#include "swift_placeholders/gpu_pack_params.h"
#include "swift_placeholders/timers.h"

#include <math.h>


/**
 * Perform the actual work of a single event.
 *
 * This region may be executed in parallel, so use atomics where
 * necessary.
 *
 * @param event: Recorded/logged event in measurement run to be reproduced in
 * this function
 * @param part_data: Struct holding pointers to all particle data
 * @param buf_dens: Particle buffer for density tasks
 * @param buf_grad: Particle buffer for gradient tasks
 * @param buf_forc: Particle buffer for force tasks
 * @param engine: the engine
 * @param timers_step: Array to store timers (ticks) for this step of the
 * simulation
 * @param timers_log_step: Array to store measured times (already converted from
 * ticks during measurement run)
 */
__attribute__((always_inline)) INLINE static void replay_event(
    const struct packing_data* event, struct part_arrays* part_data,
    struct gpu_offload_data* buf_dens, struct gpu_offload_data* buf_grad,
    struct gpu_offload_data* buf_forc, const struct engine* e,
    ticks timers_step[timer_count], double timings_log_step[timer_count]) {

  /* Get cell and fill out necessary fields */
  struct cell ci;
  init_cell(&ci, event->count, part_data->p, event->part_offset);

  const double shift[3] = {1., 1., 1.};

#ifdef SWIFT_DEBUG_CHECKS
  if ((event->part_offset + event->count) > part_data->nr_parts)
    error("Will reach beyond array length: %d %d %d %d", event->part_offset,
          event->count, event->part_offset + event->count, part_data->nr_parts);
#endif


  enum task_types type = event->task_type;
  enum task_subtypes subtype = event->task_subtype;
  if (type == task_type_pack) {

    if (subtype == task_subtype_density) {
      TIMER_TIC;
      gpu_pack_part_density(&ci, buf_dens->parts_send_d, event->pack_index,
                            shift, 0, 0);
      TIMER_TOC_LOCATION(timer_density_pack, timers_step);
      atomic_add_d(&timings_log_step[timer_density_pack], event->timing);
    } else if (subtype == task_subtype_gradient) {
      TIMER_TIC;
      gpu_pack_part_gradient(&ci, buf_grad->parts_send_g, event->pack_index,
                             shift, 0, 0);
      TIMER_TOC_LOCATION(timer_gradient_pack, timers_step);
      atomic_add_d(&timings_log_step[timer_gradient_pack], event->timing);
    } else if (subtype == task_subtype_force) {
      TIMER_TIC;
      gpu_pack_part_force(&ci, buf_grad->parts_send_f, event->pack_index, shift,
                          0, 0);
      TIMER_TOC_LOCATION(timer_force_pack, timers_step);
      atomic_add_d(&timings_log_step[timer_force_pack], event->timing);
    }

#ifdef SWIFT_DEBUG_CHECKS
    else {
      error("Unknown task subtype %d - %s", subtype, subtaskID_names[subtype]);
    }
#endif
  } else if (type == task_type_unpack) {

    if (subtype == task_subtype_density) {
      TIMER_TIC;
      gpu_unpack_part_density(&ci, buf_dens->parts_recv_d, event->pack_index,
                              event->count, e);
      TIMER_TOC_LOCATION(timer_density_unpack, timers_step);
      atomic_add_d(&timings_log_step[timer_density_unpack], event->timing);
    } else if (subtype == task_subtype_gradient) {
      TIMER_TIC;
      gpu_unpack_part_gradient(&ci, buf_dens->parts_recv_g, event->pack_index,
                               event->count, e);
      TIMER_TOC_LOCATION(timer_gradient_unpack, timers_step);
      atomic_add_d(&timings_log_step[timer_gradient_unpack], event->timing);
    } else if (subtype == task_subtype_force) {
      TIMER_TIC;
      gpu_unpack_part_force(&ci, buf_dens->parts_recv_f, event->pack_index,
                            event->count, e);
      TIMER_TOC_LOCATION(timer_force_unpack, timers_step);
      atomic_add_d(&timings_log_step[timer_force_unpack], event->timing);
    }
#ifdef SWIFT_DEBUG_CHECKS
    else {
      error("Unknown task subtype %d - %s", subtype, subtaskID_names[subtype]);
    }
#endif
  }
#ifdef SWIFT_DEBUG_CHECKS
  else {
    error("Unknown task type %d - %s", type, taskID_names[type]);
  }
#endif
}


/**
 * Run the actual thing.
 */
void run_simulation(struct parameters* params) {

  /* Allocate data to work on */
  struct part_arrays part_data;
  init_parts(&part_data, params);

  message("Starting simulation.");

  /* Init timers */
  /* Timers for a single step */
  ticks timers_step[timer_count];
  timers_reset(timers_step);

  /* Full simulation timers (summed over all steps) */
  ticks timers_full[timer_count];
  timers_reset(timers_full);

  /* Timings of the logged events for this step */
  double timing_log_step[timer_count];
  timing_reset_time(timing_log_step);

  /* Total timings of the logged events (summed over all steps) */
  double timing_log_full[timer_count];
  timing_reset_time(timing_log_full);

  /* Pretend we have an engine which has times initialised such that all
   * particles are active all the time. */
  struct engine e;
  engine_init(&e);

  /* Get packing params. Only parameter that matters here is particle buffer
   * size. */
  struct gpu_global_pack_params gpu_pack_params;
  gpu_pack_params_set(&gpu_pack_params, /*pack_size=*/1, /*bundle_size=*/1,
                      /*gpu_recursion_max_depth=*/1,
                      /*part_buffer_size=*/1000000, /*eta_neighbours=*/1.f,
                      /*nparts_hydro=*/1, /*n_top_level_cells=*/1,
                      /*nthreads=*/1);


  // - init parallel region here
  // - check that we're running on the same nr of threads as
  //   the original simulation

  // For each thread:
  int thread_id = 0;


  /* Declare and allocate GPU launch control data structures which need to be in
   * scope */
  struct gpu_offload_data gpu_buf_dens;
  struct gpu_offload_data gpu_buf_grad;
  struct gpu_offload_data gpu_buf_forc;

  gpu_data_buffers_init(&gpu_buf_dens, &gpu_pack_params,
                        sizeof(struct gpu_part_send_d),
                        sizeof(struct gpu_part_recv_d));
  gpu_data_buffers_init(&gpu_buf_grad, &gpu_pack_params,
                        sizeof(struct gpu_part_send_g),
                        sizeof(struct gpu_part_recv_g));
  gpu_data_buffers_init(&gpu_buf_forc, &gpu_pack_params,
                        sizeof(struct gpu_part_send_f),
                        sizeof(struct gpu_part_recv_f));


  /* -------------------------------------------------*/
  /* Loop over recorded simulation steps              */
  /* this loop corresponds to the 'Main loop', i.e.   */
  /* the first `while (1){}` in runner_main           */
  /* -------------------------------------------------*/
  for (int step = 0; step < params->nr_steps; step++) {

    // omp barrier

    /* Get file to read */
    char logfile[IO_MAX_FILENAME_SIZE];
    io_util_construct_log_filename(logfile, thread_id, step, params);

    /* Read trace */
    struct packing_data* packing_sequence = NULL;
    int n_events = 0;
    io_read_logged_events_file(logfile, &packing_sequence, &n_events, params);

    if (params->verbose)
      message("Thread %d step %d found %d events.", thread_id, step, n_events);


    /* ----------------------------------------------*/
    /* Loop over recorded packing tasks.             */
    /* This loop corresponds to second `while (1){}` */
    /* loop in runner_main                           */
    /* ----------------------------------------------*/
    for (int i = 0; i < n_events; i++) {
      struct packing_data event = packing_sequence[i];

      replay_event(&event, &part_data, &gpu_buf_dens, &gpu_buf_grad,
                   &gpu_buf_forc, &e, timers_step, timing_log_step);
    }

    if (params->print_each_step) print_timers(timers_step, timing_log_step);
    free(packing_sequence);

    // omp single
    for (int i = 0; i < timer_count; i++) {
      timers_full[i] += timers_step[i];
      timers_step[i] = 0;
      timing_log_full[i] += timing_log_step[i];
      timing_log_step[i] = 0.;
    }

    if (params->verbose)
      message("Thread %d finished step %d.", thread_id, step);

    // omp barrier
  }


  // Deallocate everything here.
  clear_parts(&part_data);

  message("Finished simulation.");
  print_timers(timers_full, timing_log_full);
}
