#include "io.h"
#include "error.h"
#include "help.h"

#include <dirent.h>
#include <errno.h>

/**
 * Read in cmdline args and store relevant runtime parameters
 */
void io_parse_cmdlineargs(int argc, char *argv[], struct parameters *params) {

  int data_dir_given = 0;

  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];
    if (strcmp(arg, "-v") == 0) {
      params->verbose = 1;
    } else if (strcmp(arg, "--verbose") == 0) {
      params->verbose = 1;
    } else if (strcmp(arg, "-h") == 0) {
      params->help = 1;
    } else if (strcmp(arg, "--help") == 0) {
      params->help = 1;
    } else {
      /* We may have been given the input file.
       * Check directory exists. */
      DIR *dir = opendir(arg);
      if (dir) {
        /* Directory exists. */
        closedir(dir);
        strcpy(params->data_root_dir, arg);
        data_dir_given = 1;
      } else if (ENOENT == errno) {
        /* Directory does not exist. */
        warning("Directory '%s' does not exist", arg);
      } else {
        /* opendir() failed for some other reason. */
        error("Error in opendir()");
      }
    }
  }

  if (!params->help && !data_dir_given) {
    warning("No data directory provided, that's a mandatory cmdline arg.");
    help();
  }
}

/**
 * Read in runtime parameters stored in files.
 * In particular, what was stored during the measurement runs.
 */
void io_read_params(struct parameters *params);
