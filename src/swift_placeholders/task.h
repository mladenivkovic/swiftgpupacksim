/**
 * Task data.
 */

#pragma once

#include "align.h"
#include "cycle.h"
#include "timeline.h"

/**
 * @brief The different task types.
 *
 * Be sure to update the taskID_names array in tasks.c if you modify this list!
 * Also update the python3 task plotting scripts!
 */
enum task_types {
  task_type_none = 0,
  task_type_pack,
  task_type_unpack,
  task_type_count
};

/**
 * @brief The different task sub-types (for pairs, selfs and sub-tasks).
 */
enum task_subtypes {
  task_subtype_none = 0,
  task_subtype_density,
  task_subtype_gradient,
  task_subtype_force,
  task_subtype_count,
};

/**
 * @brief Names of the task types.
 */
extern const char *taskID_names[];

/**
 * @brief Names of the task sub-types.
 */
extern const char *subtaskID_names[];


