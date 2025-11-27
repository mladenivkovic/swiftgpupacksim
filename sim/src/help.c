#include "help.h"

#include <stdio.h>
#include <stdlib.h>

void help(int errorcode) {

  printf("swiftgpupacksim - ");
  printf("simulate packing/unpacking particle data for GPUs.");
  printf("\n");
  printf("Usage:\n");
  printf("  ./swiftgpupacksim path/to/measurement/data ");
  printf("[-h] [-n] [-p] [-s N] [-v]\n\n");
  printf("   Some example measurement data should be available in the\n");
  printf("   repository. Check the swiftgpupacksim/data directory.\n\n");
  printf("  -h, --help:             print this help and exit\n");
  printf("  -n, --noflush:          Do not flush the caches after each op.\n");
  printf("      --no-flush          (Speeds simulation up significantly.)\n");
  printf("  -p, --print-each-step:  print measurements after each simulation");
  printf(" step individually\n");
  printf("  -s <nr_steps>:          run <nr_steps> steps ");
  printf("(instead of all available ones)\n");
  printf("  -v, --verbose:          increase run verbosity\n");

  exit(errorcode);
}


void banner(void) {

  printf("   ______       ____________________________  __  ______  ___   ");
  printf("________ _______ ______  ___\n");
  printf( "  / ___/ |     / /  _/ ____/_  __/ ____/ __ \\/ / / / __ \\/");
  printf("   | / ____/ //_/ ___//  _/  |/  /\n");
  printf("  \\__ \\| | /| / // // /_    / / / / __/ /_/ / / / / /_/ / /");
  printf("| |/ /   / ,<  \\__ \\ / // /|_/ / \n");
  printf(" ___/ /| |/ |/ // // __/   / / / /_/ / ____/ /_/ / ____/ ___ / ");
  printf("/___/ /| |___/ // // /  / /  \n");
  printf("/____/ |__/|__/___/_/     /_/  \\____/_/    \\____/_/   /_/  ");
  printf("|_\\____/_/ |_/____/___/_/  /_/   \n");
}
