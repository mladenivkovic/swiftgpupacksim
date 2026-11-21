/**
 * @file tests/test_particle_flavour.c
 * @brief A minimal program to verify that the generated headers of the --full
 * output are valid c code, i.e. that it compiles, for a single particle flavour.
 *
 * Look into pytests.sh for usage.
 **/

#include "src/hydro_part.h"

#include <stddef.h>

int main(void) {

  printf("Running minimal test with generated header. ");
  printf("Size of part is: %ld\n", sizeof(struct part));

  return 0;
}
