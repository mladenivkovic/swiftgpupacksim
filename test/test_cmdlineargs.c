/**
 * Test cmdline arg parsing.
 */

#include "error.h"
#include "io.h"

int main(void) {

  message("Running test_cmdlineargs");

  struct parameters params;
  init_params(&params);

  int argc1 = 2;
  char *argv1[] = {"swiftgpupack", "../data/IntelXeonGold5218_64"};
  io_parse_cmdlineargs(argc1, argv1, &params);
  assert(strcmp(params.data_root_dir, "../data/IntelXeonGold5218_64") == 0);

  init_params(&params);
  int argc2 = 3;
  char *argv2[] = {"swiftgpupack", "../data/IntelXeonGold5218_64", "-v"};
  io_parse_cmdlineargs(argc2, argv2, &params);
  assert(strcmp(params.data_root_dir, "../data/IntelXeonGold5218_64") == 0);
  assert(params.verbose == 1);
  assert(params.help == 0);

  init_params(&params);
  int argc3 = 3;
  char *argv3[] = {"swiftgpupack", "../data/IntelXeonGold5218_64", "--verbose"};
  io_parse_cmdlineargs(argc3, argv3, &params);
  assert(strcmp(params.data_root_dir, "../data/IntelXeonGold5218_64") == 0);
  assert(params.verbose == 1);
  assert(params.help == 0);

  init_params(&params);
  int argc4 = 3;
  char *argv4[] = {"swiftgpupack", "../data/IntelXeonGold5218_64", "-h"};
  io_parse_cmdlineargs(argc4, argv4, &params);
  assert(strcmp(params.data_root_dir, "../data/IntelXeonGold5218_64") == 0);
  assert(params.verbose == 0);
  assert(params.help == 1);

  init_params(&params);
  int argc5 = 3;
  char *argv5[] = {"swiftgpupack", "../data/IntelXeonGold5218_64", "--help"};
  io_parse_cmdlineargs(argc5, argv5, &params);
  assert(strcmp(params.data_root_dir, "../data/IntelXeonGold5218_64") == 0);
  assert(params.verbose == 0);
  assert(params.help == 1);

  init_params(&params);
  int argc6 = 6;
  char *argv6[] = {"swiftgpupack", "../data/IntelXeonGold5218_64",
                   "--help",       "-h",
                   "-v",           "--verbose"};
  io_parse_cmdlineargs(argc6, argv6, &params);
  assert(strcmp(params.data_root_dir, "../data/IntelXeonGold5218_64") == 0);
  assert(params.verbose == 1);
  assert(params.help == 1);

  message("Done. Bye");
}
