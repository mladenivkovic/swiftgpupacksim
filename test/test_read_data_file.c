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

  swift_assert(n_elements == 12);

  swift_assert(packing_sequence[0].task_type == task_type_density_pair);
  swift_assert(packing_sequence[0].ci_offset == 11);
  swift_assert(packing_sequence[0].ci_count == 110);
  swift_assert(packing_sequence[0].cj_offset == 12);
  swift_assert(packing_sequence[0].cj_count == 120);
  swift_assert(packing_sequence[0].timing == 1.f);

  swift_assert(packing_sequence[1].task_type == task_type_density_self);
  swift_assert(packing_sequence[1].ci_offset == 21);
  swift_assert(packing_sequence[1].ci_count == 210);
  swift_assert(packing_sequence[1].cj_offset == (unsigned long long)-1);
  swift_assert(packing_sequence[1].cj_count == -1);
  swift_assert(packing_sequence[1].timing == 2.f);

  swift_assert(packing_sequence[2].task_type == task_type_gradient_self);
  swift_assert(packing_sequence[2].ci_offset == 31);
  swift_assert(packing_sequence[2].ci_count == 310);
  swift_assert(packing_sequence[2].cj_offset == (unsigned long long)-1);
  swift_assert(packing_sequence[2].cj_count == -1);
  swift_assert(packing_sequence[2].timing == 3.f);

  swift_assert(packing_sequence[3].task_type == task_type_gradient_pair);
  swift_assert(packing_sequence[3].ci_offset == 41);
  swift_assert(packing_sequence[3].ci_count == 410);
  swift_assert(packing_sequence[3].cj_offset == 42);
  swift_assert(packing_sequence[3].cj_count == 420);
  swift_assert(packing_sequence[3].timing == 4.f);

  swift_assert(packing_sequence[4].task_type == task_type_force_self);
  swift_assert(packing_sequence[4].ci_offset == 51);
  swift_assert(packing_sequence[4].ci_count == 510);
  swift_assert(packing_sequence[4].cj_offset == (unsigned long long)-1);
  swift_assert(packing_sequence[4].cj_count == -1);
  swift_assert(packing_sequence[4].timing == 5.f);

  swift_assert(packing_sequence[5].task_type == task_type_force_pair);
  swift_assert(packing_sequence[5].ci_offset == 61);
  swift_assert(packing_sequence[5].ci_count == 610);
  swift_assert(packing_sequence[5].cj_offset == 62);
  swift_assert(packing_sequence[5].cj_count == 620);
  swift_assert(packing_sequence[5].timing == 6.f);


  free(packing_sequence);

  message("Test passed.");
}
