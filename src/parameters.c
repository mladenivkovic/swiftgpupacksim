#include "parameters.h"

#include "error.h"
#include "help.h"

/**
 * Default initialisation of the struct
 */
void init_params(struct parameters *params) {

  params->nr_threads = 0;
  params->nr_parts = 0;
  params->step = 0;
  params->nsteps = 0;

  params->verbose = 0;
  params->help = 0;

  strcpy(params->data_root_dir, "\0");
}


/**
 * Initial check that params are valid
 */
void validate_params(struct parameters *params) {

  if (params->help) {
    help();
  }
  if (params->verbose) {
    message("Starting a verbose run.");
  }
}


/**
 * Check that read-in parameters are sensible after they've
 * been read in.
 */
void validate_read_params(struct parameters *params) {

  assert(params->nr_parts > 0);
  assert(params->nr_threads > 0);

}


/**
 * Print out our run params.
 */
void print_params(struct parameters *params) {

  printf("Runtime parameters:\n");
  printf("\t nr_threads:        %3d\n", params->nr_threads);
  printf("\t nr_parts:          %3lu\n", params->nr_parts);
  /* printf("\t step:              %6d\n", params->step); */
  /* printf("\t nsteps:            %6d\n", params->nsteps); */
  printf("\t measurement data:  %s\n", params->data_root_dir);
}
