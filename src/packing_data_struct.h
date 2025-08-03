/**
 * A struct containing data of a single pack operation
 */

#pragma once

#include "task.h"

#include <stddef.h>

struct packing_data {

  /* task type */
  enum task_types task_type;

  /* ID of cell i */
  size_t ci_offset;

  /* ID of cell j */
  size_t cj_offset;

  /* particle count of cell i */
  int ci_count;

  /* particle count of cell j */
  int cj_count;

  /* Timing of the task */
  double timing;
};


inline void init_pack_data(struct packing_data *pd) {
  pd->task_type = task_type_none;
  pd->ci_offset = (unsigned long long)-1;
  pd->cj_offset = (unsigned long long)-1;
  pd->ci_count = -1;
  pd->cj_count = -1;
  pd->timing = 0.f;
}
