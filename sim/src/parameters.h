/**
 * Global runtime parameters
 */

#pragma once

#include <string.h>

struct parameters {

  /*! How many threads have we run on? */
  int nr_threads;

  /*! How many steps do we run on? */
  int nr_steps;

  /*! Are we verbose? */
  int verbose;

  /*! Are we printing out help and quitting? */
  int help;

  /*! Total number of particles in simulation */
  size_t nr_parts;

  /*! Print measurements each step? */
  int print_each_step;

  /*! Skip cache flushing? */
  int no_cache_flush;

  /*! Struct alignment */
  int struct_align;

  /*! Particle array alignment */
  int part_align;

  /*! Currently used memory layout */
  char memory_layout[40];

  /*! Root dir of measurement data */
  char data_root_dir[200];
};

void init_params(struct parameters* params);
void validate_params(struct parameters* params);
void validate_read_params(struct parameters* params);
void print_params(struct parameters* params);
