/**
 * Global runtime parameters
 */

#pragma once

#include <string.h>

struct parameters {

  // How many threads have we run on?
  int nr_threads;

  // How many steps do we run on?
  int nr_steps;

  // Are we verbose?
  int verbose;

  // Are we printing out help and quitting?
  int help;

  // Total number of particles in simulation
  size_t nr_parts;

  // Root dir of measurement data
  char data_root_dir[200];

  // Print measurements each step?
  int print_each_step;
};

void init_params(struct parameters *params);
void validate_params(struct parameters *params);
void validate_read_params(struct parameters *params);
void print_params(struct parameters *params);
