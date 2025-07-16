/**
 * Global runtime parameters
 */

#pragma once

#include <string.h>

struct parameters {

  // How many threads have we run on?
  int nthreads;

  // Which step do we run on?
  int step;

  // How many steps do we run on?
  int nsteps;

  // Are we verbose?
  int verbose;

  // Are we printing out help and quitting?
  int help;

  // Root dir of measurement data
  char data_root_dir[200];

};


void init_params(struct parameters* params);
void validate_params(struct parameters* params);

