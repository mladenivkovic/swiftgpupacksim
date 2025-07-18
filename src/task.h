/**
 * Task data.
 */

#pragma once

enum task_types {
  task_type_none = 0,
  task_type_density_self,
  task_type_density_pair,
  task_type_gradient_self,
  task_type_gradient_pair,
  task_type_force_self,
  task_type_force_pair,
  task_type_count
};


extern const char *task_type_names[task_type_count];

