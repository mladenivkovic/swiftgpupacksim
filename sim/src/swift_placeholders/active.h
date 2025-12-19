#ifndef SWIFT_ACTIVE_H
#define SWIFT_ACTIVE_H

#include "../../config.h"
#include "engine.h"
#include "parts.h"
#include "timeline.h"


/**
 * @brief Is this particle finishing its time-step now ?
 *
 * @param p The #part.
 * @param e The #engine containing information about the current time.
 * @return 1 if the #part is active, 0 otherwise.
 */
__attribute__((always_inline)) INLINE static int part_is_active(
    const struct part* p, const struct engine* e) {

  const timebin_t max_active_bin = e->max_active_bin;
  const timebin_t part_bin = part_get_time_bin(p);

#ifdef SWIFT_DEBUG_CHECKS
  const integertime_t ti_current = e->ti_current;
  const integertime_t ti_end = get_integer_time_end(ti_current, part_bin);
  if (ti_end < ti_current)
    error(
        "particle in an impossible time-zone! p->ti_end=%lld "
        "e->ti_current=%lld",
        ti_end, ti_current);
#endif

  return (part_bin <= max_active_bin);
}

#endif
