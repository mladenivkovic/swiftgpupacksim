/**
 * Task data.
 */

#include "task.h"

const char *task_type_names[task_type_count] = {
    "none",          "density_self", "density_pair", "gradient_self",
    "gradient_pair", "force_self",   "force_pair"};
