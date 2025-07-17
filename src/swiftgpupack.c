
#include "io.h"
#include "parameters.h"
#include "help.h"

int main(int argc, char *argv[]) {

  /* Initialise whatever needs to be initialized. */
  struct parameters params;
  init_params(&params);

  /* Get content of cmdline args. */
  io_parse_cmdlineargs(argc, argv, &params);
  validate_params(&params);

  /* If we're continuing, print out the banner now. */
  banner();

  /* Read parameters from run log file */
  io_read_params(&params);

  /* Show me what we're doing */
  print_params(&params);

  return 0;
}
