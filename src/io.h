/**
 * Functions related to I/O.
 */

#pragma once

#include "parameters.h"

/**
 * Read in cmdline args and store relevant runtime parameters
 */
void io_parse_cmdlineargs(int argc, char *argv[], struct parameters *params);

/**
 * Read in runtime parameters stored in files.
 * In particular, what was stored during the measurement runs.
 */
void io_read_params(struct parameters *params);
