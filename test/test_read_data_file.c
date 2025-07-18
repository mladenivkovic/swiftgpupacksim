/**
 * Test reading of run parameter file
 */

#include "error.h"
#include "io.h"
#include "packing_data_struct.h"

int main(void) {

  message("Running test_read_data_file");

  struct parameters params;
  init_params(&params);

  struct packing_data* packing_sequence = NULL;
  size_t n_elements = 0;

  io_read_logged_events_file("example_measurement.dat", &packing_sequence,
                             &n_elements, &params);

  assert(n_elements == 12);

  assert(packing_sequence[0].task_type == task_type_density_pair);
  assert(packing_sequence[0].ci_offset == 11);
  assert(packing_sequence[0].ci_count == 110);
  assert(packing_sequence[0].cj_offset == 12);
  assert(packing_sequence[0].cj_count == 120);
  assert(packing_sequence[0].timing == 1.f);

  assert(packing_sequence[1].task_type == task_type_density_self);
  assert(packing_sequence[1].ci_offset == 21);
  assert(packing_sequence[1].ci_count == 210);
  assert(packing_sequence[1].cj_offset == (unsigned long long)-1);
  assert(packing_sequence[1].cj_count == -1ul);
  assert(packing_sequence[1].timing == 2.f);

  assert(packing_sequence[2].task_type == task_type_gradient_self);
  assert(packing_sequence[2].ci_offset == 31);
  assert(packing_sequence[2].ci_count == 310);
  assert(packing_sequence[2].cj_offset == (unsigned long long)-1);
  assert(packing_sequence[2].cj_count == -1ul);
  assert(packing_sequence[2].timing == 3.f);

  assert(packing_sequence[3].task_type == task_type_gradient_pair);
  assert(packing_sequence[3].ci_offset == 41);
  assert(packing_sequence[3].ci_count == 410);
  assert(packing_sequence[3].cj_offset == 42);
  assert(packing_sequence[3].cj_count == 420);
  assert(packing_sequence[3].timing == 4.f);

  assert(packing_sequence[4].task_type == task_type_force_self);
  assert(packing_sequence[4].ci_offset == 51);
  assert(packing_sequence[4].ci_count == 510);
  assert(packing_sequence[4].cj_offset == (unsigned long long)-1);
  assert(packing_sequence[4].cj_count == -1ul);
  assert(packing_sequence[4].timing == 5.f);

  assert(packing_sequence[5].task_type == task_type_force_pair);
  assert(packing_sequence[5].ci_offset == 61);
  assert(packing_sequence[5].ci_count == 610);
  assert(packing_sequence[5].cj_offset == 62);
  assert(packing_sequence[5].cj_count == 620);
  assert(packing_sequence[5].timing == 6.f);


  free(packing_sequence);

  message("Test passed.");
}
