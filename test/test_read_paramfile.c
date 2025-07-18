/**
 * Test reading of run parameter file
 */

#include "error.h"
#include "io.h"
#include "parameters.h"

int main(void) {

  message("Running test_read_paramfile");

  struct parameters params;
  init_params(&params);

  int argc = 2;
  char *argv[] = {"swiftpacksim", "."};
  io_parse_cmdlineargs(argc, argv, &params);
  validate_params(&params);

  io_read_logged_params(&params);
  validate_read_params(&params);
  assert(params.nr_threads == 32);

  message("Test passed.");
}
