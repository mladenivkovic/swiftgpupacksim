/**
 * This is where the party is at.
 */

#include "clocks.h"
#include "error.h"
#include "help.h"
#include "io.h"
#include "parameters.h"
#include "run_simulation.h"


int main(int argc, char* argv[]) {

  /* Initialise whatever needs to be initialized. */
  struct parameters params;
  init_params(&params);

  /* Get content of cmdline args. */
  io_parse_cmdlineargs(argc, argv, &params);
  validate_params(&params);

  /* If we're continuing, print out the banner now. */
  banner();

  /* Genesis 1.1: And then, there was time ! */
  clocks_set_cpufreq(0);
  if (params.verbose) {
    message("CPU frequency used for tick conversion: %llu Hz",
            clocks_get_cpufreq());
  }

  /* Read parameters from run log file */
  io_read_logged_params(&params);

  /* Show me what we're doing */
  print_params(&params);

  /* Let's get to work. */
  run_simulation(&params);

  return 0;
}
