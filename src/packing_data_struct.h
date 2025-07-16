/**
 * A struct containing data of a single pack operation
 */

#pragma once

#include "task.h"

struct packing_data {

  // task type
  enum task_types task_type;

  // ID of cell i
  unsigned long long ci_cellID;

  // ID of cell j
  unsigned long long cj_cellID;

  // particle count of cell i
  int ci_count;

  // particle count of cell j
  int cj_count;
};


inline void init_pack_data(struct packing_data* pd){
  pd->task_type = task_type_none;
  pd->ci_cellID = (unsigned long long) -1;
  pd->cj_cellID = (unsigned long long) -1;
  pd->ci_cellID = -1;
  pd->cj_cellID = -1;
}
