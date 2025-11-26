#include "help.h"

#include <stdio.h>
#include <stdlib.h>

void help(void) {

  printf("swiftgpupacksim - ");
  printf("simulate packing/unpacking particle data for GPUs.");
  printf("\n");
  printf("Usage:\n");
  printf("  ./swiftgpupacksim path/to/measurement/data [-v -h]");
  printf("\n\n");
  printf( "   Some measurement data should be available in the repository.");
  printf("\n\n");
  printf("  -h, --help:             print this help and exit\n");
  printf("  -p, --print-each-step:  print measurements after each simulation");
  printf(" step individually\n");
  printf("  -v, --verbose:          Increase run verbosity\n");

  exit(0);
}


void banner(void) {

  printf(
      "   ______       ____________________________  __  ______  ___   "
      "________ _______ ______  ___\n");
  printf(
      "  / ___/ |     / /  _/ ____/_  __/ ____/ __ \\/ / / / __ \\/   | / "
      "____/ //_/ ___//  _/  |/  /\n");
  printf(
      "  \\__ \\| | /| / // // /_    / / / / __/ /_/ / / / / /_/ / /| |/ /   / "
      ",<  \\__ \\ / // /|_/ / \n");
  printf(
      " ___/ /| |/ |/ // // __/   / / / /_/ / ____/ /_/ / ____/ ___ / /___/ /| "
      "|___/ // // /  / /  \n");
  printf(
      "/____/ |__/|__/___/_/     /_/  \\____/_/    \\____/_/   /_/  "
      "|_\\____/_/ |_/____/___/_/  /_/   \n");
}
