#include "parameters.h"

#include "error.h"
#include "help.h"

void init_params(struct parameters *params) {

  params->nthreads = 0;
  params->step = 0;
  params->nsteps = 0;

  params->verbose = 0;
  params->help = 0;

  strcpy(params->data_root_dir, "\0");
}

void validate_params(struct parameters *params) {

  if (params->help) {
    help();
  }
  if (params->verbose) {
    message("Starting a verbose run.");
  }
}


void print_params(struct parameters *params){

  printf("Runtime parameters:\n");
  printf("\t nr_threads:        %3d\n", params->nthreads);
  /* printf("\t step:              %6d\n", params->step); */
  /* printf("\t nsteps:            %6d\n", params->nsteps); */
  printf("\t measurement data:  %s\n", params->data_root_dir);

}
