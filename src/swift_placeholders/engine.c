#include "engine.h"
#include "error.h"


/**
 * Initialize the engine struct such that all particles are active all the
 * time.
 * */
void engine_init(struct engine* e){
  e->max_active_bin = num_time_bins;
  e->ti_current = 0;
}
