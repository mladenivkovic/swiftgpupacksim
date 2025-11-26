/**
 * A struct containing data of a single pack operation
 */

#pragma once

#include "swift_placeholders/task.h"

#include <stddef.h>

struct packing_data {

  /* task subtype: d,g,f */
  enum task_subtypes task_subtype;

  /* task type: pack, unpack */
  enum task_types task_type;

  /* ID of cell i */
  int part_offset;

  /* particle count of cell i */
  int count;

  /* index where to pack from/to */
  int pack_index;

  /* Timing of the task */
  double timing;
} __attribute__((aligned(32)));


__attribute__((always_inline)) inline void init_pack_data(
    struct packing_data *pd) {
  pd->task_type = task_type_none;
  pd->task_subtype = task_subtype_none;
  pd->part_offset = -1;
  pd->count = -1;
  pd->pack_index = -1;
  pd->timing = 0.;
}
