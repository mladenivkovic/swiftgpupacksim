/**
 * This is where the party is at.
 */

#include "clocks.h"
#include "help.h"
#include "io.h"
#include "packing_data_struct.h"
#include "parameters.h"
#include "parts.h"
#include "timers.h"


/**
 * Dummy function for now.
 *
 * @param data: Struct holding pointers to all particle data
 * @param ci_offset: index at which this cell's particles start in global particle array
 * @param count: nr of particles in this cell
 * @param nr_parts: total number of particles in this simulation
 */
void pack_cell(struct part_arrays* data, size_t ci_offset, size_t count) {

#ifdef SWIFT_DEBUG_CHECKS
  if ((ci_offset + count) > data->nr_parts)
    error("Will reach beyond array length: %lu %lu %lu %lu", ci_offset, count, ci_offset+count, data->nr_parts);
#endif

  for (size_t i = ci_offset; i < ci_offset + count; i++) {
    struct part* p = &data->p[i];
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
void do_work(const struct packing_data* event, struct part_arrays* data,
             ticks timers_step[timer_count],
             double timings_log_step[timer_count]) {

  enum task_types type = event->task_type;
  if (type == task_type_density_self) {
    TIMER_TIC;
    pack_cell(data, event->ci_offset, event->ci_count);
    TIMER_TOC_LOCATION(timer_density_self, timers_step);
    atomic_add_d(&timings_log_step[timer_density_self], event->timing);
  } else if (type == task_type_gradient_self) {
    TIMER_TIC;
    pack_cell(data, event->ci_offset, event->ci_count);
    TIMER_TOC_LOCATION(timer_gradient_self, timers_step);
    atomic_add_d(&timings_log_step[timer_gradient_self], event->timing);
  } else if (type == task_type_force_self) {
    TIMER_TIC;
    pack_cell(data, event->ci_offset, event->ci_count);
    TIMER_TOC_LOCATION(timer_force_self, timers_step);
    atomic_add_d(&timings_log_step[timer_force_self], event->timing);
  } else if (type == task_type_density_pair) {
    TIMER_TIC;
    pack_cell(data, event->ci_offset, event->ci_count);
    pack_cell(data, event->cj_offset, event->cj_count);
    TIMER_TOC_LOCATION(timer_density_pair, timers_step);
    atomic_add_d(&timings_log_step[timer_density_pair], event->timing);
  } else if (type == task_type_gradient_pair) {
    TIMER_TIC;
    pack_cell(data, event->ci_offset, event->ci_count);
    pack_cell(data, event->cj_offset, event->cj_count);
    TIMER_TOC_LOCATION(timer_gradient_pair, timers_step);
    atomic_add_d(&timings_log_step[timer_gradient_pair], event->timing);
  } else if (type == task_type_force_pair) {
    TIMER_TIC;
    pack_cell(data, event->ci_offset, event->ci_count);
    pack_cell(data, event->cj_offset, event->cj_count);
    TIMER_TOC_LOCATION(timer_force_pair, timers_step);
    atomic_add_d(&timings_log_step[timer_force_pair], event->timing);
  } else {
    error("Unknown task type");
  }
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

  for (int step = 0; step < params->nr_steps; step++){

    // For each thread:
    int thread_id = 0;

    /* Get file to read */
    char logfile[IO_MAX_FILENAME_SIZE];
    io_util_construct_log_filename(logfile, 0, step, params);

    /* Read trace */
    struct packing_data* packing_sequence = NULL;
    size_t n_events = 0;
    io_read_logged_events_file(logfile, &packing_sequence, &n_events, params);

    if (params->verbose)
      message("Thread %d step %d found %lu events.", thread_id, step, n_events);

    for (size_t i = 0; i < n_events; i++) {
      struct packing_data event = packing_sequence[i];
      do_work(&event, &part_data, timers_step, timing_log_step);
    }

    message("Finished step %d", step);
    print_timers(timers_step, timing_log_step);
    free(packing_sequence);

    // omp single
    for (int i = 0; i < timer_count; i++){
      timers_full[i] += timers_step[i];
      timers_step[i] = 0;
      timing_log_full[i] += timing_log_step[i];
      timing_log_step[i] = 0.;
    }
    // omp barrier
  }

  message("Finished simulation.");
  print_timers(timers_full, timing_log_full);

  clear_parts(&part_data);
}


int main(int argc, char* argv[]) {

  /* Initialise whatever needs to be initialized. */
  struct parameters params;
  init_params(&params);

  /* Get content of cmdline args. */
  io_parse_cmdlineargs(argc, argv, &params);
  validate_params(&params);

  /* If we're continuing, print out the banner now. */
  banner();

  /* Genesis 1.1: And then, there was time ! */
  clocks_set_cpufreq(0);
  if (params.verbose) {
    message("CPU frequency used for tick conversion: %llu Hz",
            clocks_get_cpufreq());
  }

  /* Read parameters from run log file */
  io_read_logged_params(&params);

  /* Show me what we're doing */
  print_params(&params);

  /* Let's get to work. */
  run_simulation(&params);


  return 0;
}
