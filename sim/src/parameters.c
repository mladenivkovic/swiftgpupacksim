#include "parameters.h"

#include "help.h"
#include "swift_placeholders/error.h"

/**
 * Default initialisation of the struct
 */
void init_params(struct parameters* params) {

  params->nr_threads = 0;
  params->nr_parts = 0;
  params->nr_steps = 0;

  params->verbose = 0;
  params->help = 0;

  params->print_each_step = 0;
  params->no_cache_flush = 0;

  params->struct_align = SWIFT_STRUCT_ALIGNMENT;
  params->part_align = SWIFT_PART_ALIGNMENT;

#if defined(SPHENIX_AOS_PARTICLE)
  char layout[20] = "aos";
#elif defined(SPHENIX_SOA_PARTICLE)
#if defined(MODIFIED_PARTICLE_ACCESS)
  char layout[20] = "soa-modified";
#else
  char layout[20] = "soa";
#endif
#elif defined(SPHENIX_UPSTREAM_PARTICLE)
  char layout[20] = "upstream";
#else
#pragma error "Unknown particle memory layout"
#endif
  strcpy(params->memory_layout, layout);

  strcpy(params->data_root_dir, "\0");
}


/**
 * Initial check that params are valid
 */
void validate_params(struct parameters* params) {

  if (params->help) {
    help(0);
  }
  if (params->verbose) {
    message("Starting a verbose run.");
  }
}


/**
 * Check that read-in parameters are sensible after they've
 * been read in.
 */
void validate_read_params(struct parameters* params) {

  swift_assert(params->nr_parts > 0);
  swift_assert(params->nr_threads > 0);
  swift_assert(params->nr_steps > 0);
}


/**
 * Print out our run params.
 */
void print_params(struct parameters* params) {

  printf("Runtime parameters:\n");
  printf("\t nr_threads:        %12d\n", params->nr_threads);
  printf("\t nr_parts:          %12lu\n", params->nr_parts);
  printf("\t nr_steps:          %12d\n", params->nr_steps);
  printf("\t cache flushing:    %12d\n", !params->no_cache_flush);
  printf("\t struct alignment:  %12d\n", params->struct_align);
  printf("\t part array align:  %12d\n", params->part_align);
  printf("\t memory layout:     %s\n", params->memory_layout);
  printf("\t measurement data:  %s\n", params->data_root_dir);
}
