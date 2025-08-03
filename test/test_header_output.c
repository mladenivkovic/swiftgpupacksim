/* A minimal program to verify that the generated headers are valid c code. */

#include <stdio.h>

#include "hydro_part.h"

int main(void){

  printf("Running minimal test with generated header. ");
  printf("Size of part is: %ld\n", sizeof(struct part));

  return 0;
}
