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
