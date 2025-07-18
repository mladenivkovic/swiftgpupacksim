#pragma once

#include "hydro_part.h"
#include "parameters.h"

/**
 * Struct holding handles to all particle data arrays
 */
struct part_arrays {
  struct part* p;

#ifdef SWIFT_DEBUG_CHECKS
  size_t nr_parts;
#endif
};


/**
 * Initialize particle data array(s).
 */
void init_parts(struct part_arrays* data, const struct parameters* params) {

  data->p = calloc(params->nr_parts, sizeof(struct part));

#ifdef SWIFT_DEBUG_CHECKS
  data->nr_parts = params->nr_parts;
#endif

  message("size of part is %lu bytes", sizeof(struct part));
  message("size of alloc'd part is %.3g Mb",
          (params->nr_parts * sizeof(struct part)) / (1024. * 1024.));
}


/**
 * Free the bytes from their prison of labour.
 */
void clear_parts(struct part_arrays* data) { free(data->p); }
