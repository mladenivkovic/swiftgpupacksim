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
  int n_elements = 0;

  io_read_logged_events_file("input/example_measurement.dat", &packing_sequence,
                             &n_elements, &params);

  swift_assert(n_elements == 6);

  swift_assert(packing_sequence[0].task_type == task_type_pack);
  swift_assert(packing_sequence[0].task_subtype == task_subtype_density);
  swift_assert(packing_sequence[0].part_offset == 11);
  swift_assert(packing_sequence[0].count == 12);
  swift_assert(packing_sequence[0].pack_index == 13);
  swift_assert(packing_sequence[0].timing == 1.f);

  swift_assert(packing_sequence[1].task_type == task_type_unpack);
  swift_assert(packing_sequence[1].task_subtype == task_subtype_density);
  swift_assert(packing_sequence[1].part_offset == 21);
  swift_assert(packing_sequence[1].count == 22);
  swift_assert(packing_sequence[1].pack_index == 23);
  swift_assert(packing_sequence[1].timing == 2.f);

  swift_assert(packing_sequence[2].task_type == task_type_pack);
  swift_assert(packing_sequence[2].task_subtype == task_subtype_gradient);
  swift_assert(packing_sequence[2].part_offset == 31);
  swift_assert(packing_sequence[2].count == 32);
  swift_assert(packing_sequence[2].pack_index == 33);
  swift_assert(packing_sequence[2].timing == 3.f);

  swift_assert(packing_sequence[3].task_type == task_type_unpack);
  swift_assert(packing_sequence[3].task_subtype == task_subtype_gradient);
  swift_assert(packing_sequence[3].part_offset == 41);
  swift_assert(packing_sequence[3].count == 42);
  swift_assert(packing_sequence[3].pack_index == 43);
  swift_assert(packing_sequence[3].timing == 4.f);

  swift_assert(packing_sequence[4].task_type == task_type_pack);
  swift_assert(packing_sequence[4].task_subtype == task_subtype_force);
  swift_assert(packing_sequence[4].part_offset == 51);
  swift_assert(packing_sequence[4].count == 52);
  swift_assert(packing_sequence[4].pack_index == 53);
  swift_assert(packing_sequence[4].timing == 5.f);

  swift_assert(packing_sequence[5].task_type == task_type_unpack);
  swift_assert(packing_sequence[5].task_subtype == task_subtype_force);
  swift_assert(packing_sequence[5].part_offset == 61);
  swift_assert(packing_sequence[5].count == 62);
  swift_assert(packing_sequence[5].pack_index == 63);
  swift_assert(packing_sequence[5].timing == 6.f);

  free(packing_sequence);

  message("Test passed.");
}
