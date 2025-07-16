#include "help.h"

#include <stdio.h>
#include <stdlib.h>

void help(void) {

  printf(
      "swiftgpupacksim - simulate packing/unpacking particle data for GPUs.\n");
  printf("Usage:\n");
  printf("    ./swiftgpupacksim path/to/measurement/data [-v -h]\n\n");
  printf(
      "     Some measurement data should be available in the repository.\n\n");
  printf("    -v, --verbose: Increase run verbosity\n");
  printf("    -h, --help:    print this help and exit\n");

  exit(0);
}
