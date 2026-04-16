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
  char layout[20] = "soa";
#elif defined(SPHENIX_UPSTREAM_PARTICLE)
  char layout[20] = "upstream";
#elif defined(SPHENIX_PACK_GRADIENT_PARTICLE)
  char layout[20] = "pack-gradient";
#elif defined(SPHENIX_PACK_FORCE_PARTICLE)
  char layout[20] = "pack-force";
#elif defined(SPHENIX_PACK_SHARED_PARTICLE)
  char layout[20] = "pack-shared";
#else
#pragma error "Unknown particle memory layout"
#endif
  strcpy(params->memory_layout, layout);

#if defined(SWIFT_PARTICLE_ACCESS_PART_STRUCT)
  char access_variant[20] = "part-struct";
#elif defined(SWIFT_PARTICLE_ACCESS_GLOBAL_VAR)
  char access_variant[20] = "global-var";
#elif defined(SWIFT_PARTICLE_ACCESS_EXPLICIT_VAR)
  char access_variant[20] = "explicit-var";
#else
#pragma error "Unknown particle access variant"
#endif
  strcpy(params->access_variant, access_variant);

#if defined(SWIFT_LOOP_SPLIT_NONE)
  char loop_split[20] = "none";
#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT)
  char loop_split[20] = "by-struct";
#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT)
  char loop_split[20] = "by-element";
#else
#pragma error "Unknown loop splitting variant"
#endif
  strcpy(params->loop_split, loop_split);

  /* Empty string for intial value of data root dir */
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

#ifdef VECTORIZE
  const int vec = 1;
#else
  const int vec = 0;
#endif

#ifdef PACKED_STRUCTS
  const int packed = 1;
#else
  const int packed = 0;
#endif


  printf("Runtime parameters:\n");
  printf("\t nr_threads:             %12d\n", params->nr_threads);
  printf("\t nr_parts:               %12lu\n", params->nr_parts);
  printf("\t nr_steps:               %12d\n", params->nr_steps);
  printf("\t cache flush:            %12d\n", !params->no_cache_flush);
  printf("\t stuct align:            %12d\n", params->struct_align);
  printf("\t part array align:       %12d\n", params->part_align);
  printf("\t memory layout:          %s\n", params->memory_layout);
  printf("\t access variant:         %s\n", params->access_variant);
  printf("\t loop splitting variant: %s\n", params->loop_split);
  printf("\t vectorization:          %12d\n", vec);
  printf("\t packed structs:         %12d\n", packed);
  printf("\t measurement data:       %s\n", params->data_root_dir);
}
