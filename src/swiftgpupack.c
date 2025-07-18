/**
 * This is where the party is at.
 */

#include "help.h"
#include "io.h"
#include "packing_data_struct.h"
#include "parameters.h"
#include "parts.h"


void pack_cell(struct part_arrays* data, size_t ci_offset, size_t count){

  for (size_t i = ci_offset; i < ci_offset + count; i++){
    struct part* p = &data->p[i];
    part_set_id(p, part_get_id(p) + 1);
  }

}


void do_work(const struct packing_data* event, struct part_arrays* data){

  enum task_types type = event->task_type;
  if (type == task_type_density_self || type == task_type_gradient_self || type == task_type_force_self){
    pack_cell(data, event->ci_offset, event->ci_count);
  }
  else if (type == task_type_density_pair || type == task_type_gradient_pair || type == task_type_force_pair){
    pack_cell(data, event->ci_offset, event->ci_count);
    pack_cell(data, event->cj_offset, event->cj_count);
  } else {
    error("Unknown task type");
  }

}


/**
 * Run the actual thing.
 */
void run_simulation(struct parameters *params){

  /* Allocate data to work on */
  struct part_arrays part_data;
  init_parts(&part_data, params);

  message("Starting simulation.");

  // TODO(mivkov):
  // - loop over steps
  int step = 0;

  // - init parallel region here
  // - check that we're running on the same nr of threads as
  //   the original simulation


  // For each thread:

  /* Get file to read */
  char logfile[IO_MAX_FILENAME_SIZE];
  io_util_construct_log_filename(logfile, 0, step, params);

  /* Read trace */
  struct packing_data* packing_sequence = NULL;
  size_t n_events = 0;
  io_read_measurement_file(logfile, &packing_sequence, &n_events, params);

  for (size_t i = 0; i < n_events; i++){
    struct packing_data event = packing_sequence[i];
    do_work(&event, &part_data);
  }

  message("Finished step %d", step);

  clear_parts(&part_data);
}





int main(int argc, char *argv[]) {

  /* Initialise whatever needs to be initialized. */
  struct parameters params;
  init_params(&params);

  /* Get content of cmdline args. */
  io_parse_cmdlineargs(argc, argv, &params);
  validate_params(&params);

  /* If we're continuing, print out the banner now. */
  banner();

  /* Read parameters from run log file */
  io_read_params(&params);

  /* Show me what we're doing */
  print_params(&params);

  /* Let's get to work. */
  run_simulation(&params);


  return 0;
}
