/**
 * Main function that re-runs the recorded simulation.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "parameters.h"

#include <vector_types.h>

void run_simulation(struct parameters *params);

#ifdef __cplusplus
}
#endif
