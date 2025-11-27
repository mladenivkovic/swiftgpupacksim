#include "run_simulation.h"

#include "io.h"
#include "parts.h"
#include "swift_placeholders/clocks.h"
#include "swift_placeholders/cuda/gpu_offload_data.h"
#include "swift_placeholders/cuda/gpu_part_pack_functions.h"
#include "swift_placeholders/engine.h"
#include "swift_placeholders/gpu_pack_params.h"
#include "swift_placeholders/timers.h"

#include <float.h>
#include <omp.h>

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
 * @param timings_log_step: Array to store measured times (already converted
 * from ticks during measurement run)
 * @param timing_ratio_min: Array to store smallest ratio between simulated and
 * measured timings
 * @param timing_ratio_max: Array to store biggest ratio between simulated and
 * measured timings
 * @param garbage A large array O(MB) to fill out with garbage data to flush
 * the caches after each op
 * @param n_garbage: size of garbage array
 *
 * @return garbage_sum Some garbage double; Needed to prevent compiler from
 * optimising out a big loop doing nothing but flushing caches
 */
__attribute__((always_inline)) INLINE static double replay_event(
    const struct packing_data* event, struct part_arrays* part_data,
    struct gpu_offload_data* buf_dens, struct gpu_offload_data* buf_grad,
    struct gpu_offload_data* buf_forc, const struct engine* e,
    ticks timers_step[timer_count], double timings_log_step[timer_count],
    double timing_ratio_min[timer_count], double timing_ratio_max[timer_count],
    double* garbage, int n_garbage) {

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

      const ticks tic = getticks();
      gpu_pack_part_density(&ci, buf_dens->parts_send_d, event->pack_index,
                            shift, 0, 0);
      const ticks toc = getticks();

      atomic_add(&timers_step[timer_density_pack], toc - tic);
      atomic_add_d(&timings_log_step[timer_density_pack], event->timing);

      const double dt = clocks_diff_ticks(toc, tic) * 1e3 / event->timing;
      atomic_min_d(&timing_ratio_min[timer_density_pack], dt);
      atomic_max_d(&timing_ratio_max[timer_density_pack], dt);

    } else if (subtype == task_subtype_gradient) {

      const ticks tic = getticks();
      gpu_pack_part_gradient(&ci, buf_grad->parts_send_g, event->pack_index,
                             shift, 0, 0);
      const ticks toc = getticks();

      atomic_add(&timers_step[timer_gradient_pack], toc - tic);
      atomic_add_d(&timings_log_step[timer_gradient_pack], event->timing);

      const double dt = clocks_diff_ticks(toc, tic) * 1e3 / event->timing;
      atomic_min_d(&timing_ratio_min[timer_gradient_pack], dt);
      atomic_max_d(&timing_ratio_max[timer_gradient_pack], dt);

    } else if (subtype == task_subtype_force) {

      const ticks tic = getticks();
      gpu_pack_part_force(&ci, buf_forc->parts_send_f, event->pack_index, shift,
                          0, 0);
      const ticks toc = getticks();

      atomic_add(&timers_step[timer_force_pack], toc - tic);
      atomic_add_d(&timings_log_step[timer_force_pack], event->timing);

      const double dt = clocks_diff_ticks(toc, tic) * 1e3 / event->timing;
      atomic_min_d(&timing_ratio_min[timer_force_pack], dt);
      atomic_max_d(&timing_ratio_max[timer_force_pack], dt);

    }

#ifdef SWIFT_DEBUG_CHECKS
    else {
      error("Unknown task subtype %d - %s", subtype, subtaskID_names[subtype]);
    }
#endif
  } else if (type == task_type_unpack) {

    if (subtype == task_subtype_density) {

      const ticks tic = getticks();
      gpu_unpack_part_density(&ci, buf_dens->parts_recv_d, event->pack_index,
                              event->count, e);
      const ticks toc = getticks();

      atomic_add(&timers_step[timer_density_unpack], toc - tic);
      atomic_add_d(&timings_log_step[timer_density_unpack], event->timing);

      const double dt = clocks_diff_ticks(toc, tic) * 1e3 / event->timing;
      atomic_min_d(&timing_ratio_min[timer_density_unpack], dt);
      atomic_max_d(&timing_ratio_max[timer_density_unpack], dt);

    } else if (subtype == task_subtype_gradient) {

      const ticks tic = getticks();
      gpu_unpack_part_gradient(&ci, buf_grad->parts_recv_g, event->pack_index,
                               event->count, e);
      const ticks toc = getticks();

      atomic_add(&timers_step[timer_gradient_unpack], toc - tic);
      atomic_add_d(&timings_log_step[timer_gradient_unpack], event->timing);

      const double dt = clocks_diff_ticks(toc, tic) * 1e3 / event->timing;
      atomic_min_d(&timing_ratio_min[timer_gradient_unpack], dt);
      atomic_max_d(&timing_ratio_max[timer_gradient_unpack], dt);

    } else if (subtype == task_subtype_force) {

      const ticks tic = getticks();
      gpu_unpack_part_force(&ci, buf_forc->parts_recv_f, event->pack_index,
                            event->count, e);
      const ticks toc = getticks();

      atomic_add(&timers_step[timer_force_unpack], toc - tic);
      atomic_add_d(&timings_log_step[timer_force_unpack], event->timing);

      const double dt = clocks_diff_ticks(toc, tic) * 1e3 / event->timing;
      atomic_min_d(&timing_ratio_min[timer_force_unpack], dt);
      atomic_max_d(&timing_ratio_max[timer_force_unpack], dt);

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

  double sum = 0.;
  for (int i = 0; i < n_garbage; i++) {
    garbage[i] = (2. * i - 13.) * 4.;
    sum += garbage[i];
  }
  return sum;
}


/**
 * Run the actual thing.
 */
void run_simulation(struct parameters* params) {

  /* Allocate data to work on */
  struct part_arrays part_data;
  init_parts(&part_data, params);

  message("Starting simulation.");

  /* Get devnull handle to dump garbage used for cache flushing in */
  FILE* devnull = fopen("/dev/null", "w");
  if (devnull == NULL) error("Oh no :(");

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

  /* Get min/max ratios of individual packing/unpackings too */
  double timing_ratio_min[timer_count];
  for (int i = 0; i < timer_count; i++) timing_ratio_min[i] = DBL_MAX;
  double timing_ratio_max[timer_count];
  for (int i = 0; i < timer_count; i++) timing_ratio_max[i] = -1.;

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


#pragma omp parallel num_threads(params->nr_threads)
{

  /* Double-check nothing's overwriting what we want to do. */
#ifdef SWIFT_DEBUG_CHECKS
  int n_threads = omp_get_num_threads();
  if (n_threads != params->nr_threads) {
    error("Started a parallel region with %d threads instead of %d", n_threads, params->nr_threads);
  }
#endif

  /* Alloc a few MB of data and fill them with garbage to flush caches. */
  const int n_garbage = params->no_cache_flush ? 0 : 250000;
  double* garbage = malloc(n_garbage * sizeof(double));

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
#pragma omp barrier
  for (int step = 0; step < params->nr_steps; step++) {

    /* Use a single omp thread per thread of the original run */
#pragma omp for schedule(static,1)
    for (int thread_id = 0; thread_id < params->nr_threads; thread_id++) {

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
      double garbage_sum = 0.;
      for (int i = 0; i < n_events; i++) {
        struct packing_data event = packing_sequence[i];

        double g =
            replay_event(&event, &part_data, &gpu_buf_dens, &gpu_buf_grad,
                         &gpu_buf_forc, &e, timers_step, timing_log_step,
                         timing_ratio_min, timing_ratio_max, garbage, n_garbage);
        garbage_sum += g;
      }

      /* Write it into nothingness. */
      fprintf(devnull, "garbage: %g", garbage_sum);

      free(packing_sequence);

      if (params->verbose)
        message("Thread %d finished step %d.", thread_id, step);

    } /* (parallel) loop over thread_ids */

#pragma omp barrier
#pragma omp master
{
  message("Finished step %d adding up", step);
    for (int i = 0; i < timer_count; i++) {
      timers_full[i] += timers_step[i];
      timers_step[i] = 0;
      timing_log_full[i] += timing_log_step[i];
      timing_log_step[i] = 0.;
    }

    if (params->print_each_step)
      io_print_timers(timers_step, timing_log_step, timing_ratio_min, timing_ratio_max);
}

#pragma omp barrier
  }

  /* Clean up after yourself */
  free(garbage);
  gpu_data_buffers_free(&gpu_buf_dens);
  gpu_data_buffers_free(&gpu_buf_grad);
  gpu_data_buffers_free(&gpu_buf_forc);

} /* omp parallel */

  /* Clean up after yourself */
  clear_parts(&part_data);
  fclose(devnull);

  message("Finished simulation.");
  io_print_timers(timers_full, timing_log_full, timing_ratio_min,
                  timing_ratio_max);
}
