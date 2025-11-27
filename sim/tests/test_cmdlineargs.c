/**
 * Test cmdline arg parsing.
 */

#include "error.h"
#include "io.h"
#include "version_string.h"

#include <string.h>

int main(void) {

  message("Running test_cmdlineargs");

  struct parameters params;
  init_params(&params);

  char datadir[200];
  strcpy(datadir, REPO_ROOT_DIR);
  strcat(datadir, "/data/IntelXeonGold5218_Gresho64_1thread");

  int argc1 = 2;
  char *argv1[] = {"swiftgpupack", datadir};
  io_parse_cmdlineargs(argc1, argv1, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);

  init_params(&params);
  int argc2 = 3;
  char *argv2[] = {"swiftgpupack", datadir, "-v"};
  io_parse_cmdlineargs(argc2, argv2, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.verbose == 1);
  swift_assert(params.help == 0);

  init_params(&params);
  int argc3 = 3;
  char *argv3[] = {"swiftgpupack", datadir, "--verbose"};
  io_parse_cmdlineargs(argc3, argv3, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.verbose == 1);
  swift_assert(params.help == 0);

  init_params(&params);
  int argc4 = 3;
  char *argv4[] = {"swiftgpupack", datadir, "-h"};
  io_parse_cmdlineargs(argc4, argv4, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.verbose == 0);
  swift_assert(params.help == 1);

  init_params(&params);
  int argc5 = 3;
  char *argv5[] = {"swiftgpupack", datadir, "--help"};
  io_parse_cmdlineargs(argc5, argv5, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.verbose == 0);
  swift_assert(params.help == 1);

  init_params(&params);
  int argc6 = 6;
  char *argv6[] = {"swiftgpupack", datadir, "--help","-h", "-v", "--verbose"};
  io_parse_cmdlineargs(argc6, argv6, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.verbose == 1);
  swift_assert(params.help == 1);

  init_params(&params);
  int argc7 = 4;
  char *argv7[] = {"swiftgpupack", datadir, "-s","1234"};
  io_parse_cmdlineargs(argc7, argv7, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.nr_steps == 1234);

  init_params(&params);
  int argc8 = 3;
  char *argv8[] = {"swiftgpupack", datadir, "--no-flush"};
  io_parse_cmdlineargs(argc8, argv8, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.no_cache_flush == 1);

  init_params(&params);
  int argc9 = 3;
  char *argv9[] = {"swiftgpupack", datadir, "--noflush"};
  io_parse_cmdlineargs(argc9, argv9, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.no_cache_flush == 1);

  init_params(&params);
  int argc10 = 3;
  char *argv10[] = {"swiftgpupack", datadir, "-n"};
  io_parse_cmdlineargs(argc10, argv10, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.no_cache_flush == 1);

  init_params(&params);
  int argc11 = 3;
  char *argv11[] = {"swiftgpupack", datadir, "-p"};
  io_parse_cmdlineargs(argc11, argv11, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.print_each_step == 1);

  init_params(&params);
  int argc12 = 3;
  char *argv12[] = {"swiftgpupack", datadir, "--print-each-step"};
  io_parse_cmdlineargs(argc12, argv12, &params);
  swift_assert(strcmp(params.data_root_dir, datadir) == 0);
  swift_assert(params.print_each_step == 1);

  message("Test passed.");
}
