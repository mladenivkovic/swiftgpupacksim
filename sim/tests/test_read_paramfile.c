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

  /* Fake cmdlineargs. */
  int argc = 2;
  /* First argument: fake executable name. */
  char *argv[] = {"swiftpacksim", "input"};
  io_parse_cmdlineargs(argc, argv, &params);
  validate_params(&params);

  io_read_logged_params(&params);
  validate_read_params(&params);
  swift_assert(params.nr_threads == 32);
  swift_assert(params.nr_steps == 3);
  swift_assert(params.nr_parts == 101);

  message("Test passed.");
}
