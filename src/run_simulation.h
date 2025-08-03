/**
 * Main function that re-runs the recorded simulation.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cell.h"
#include "cuda/pack_vars.h"
#include "cuda/part_gpu.h"
#include "packing_data_struct.h"
#include "parameters.h"
#include "parts.h"
#include "runner.h"
#include "scheduler.h"
#include "timers.h"

#include <vector_types.h>


void pack_cell_dummy(struct part_arrays *data, size_t ci_offset, size_t count);

void replay_event(const struct packing_data *event, struct part_arrays *data,
                  struct runner *r, struct cell *ci, struct cell *cj,
                  struct task *t, struct scheduler *sched,
                  struct pack_vars_self *pack_vars_self_dens,
                  struct pack_vars_pair *pack_vars_pair_dens,
                  struct pack_vars_self *pack_vars_self_grad,
                  struct pack_vars_pair *pack_vars_pair_grad,
                  struct pack_vars_self *pack_vars_self_forc,
                  struct pack_vars_pair *pack_vars_pair_forc,
                  int2 *task_first_part_f4, int2 *task_first_part_f4_g,
                  int2 *task_first_part_f4_f,
                  struct part_aos_f4_send *parts_aos_f4_send,
                  struct part_aos_f4_recv *parts_aos_f4_recv,
                  struct part_aos_f4_g_send *parts_aos_grad_f4_send,
                  struct part_aos_f4_g_recv *parts_aos_grad_f4_recv,
                  struct part_aos_f4_f_send *parts_aos_forc_f4_send,
                  struct part_aos_f4_f_recv *parts_aos_forc_f4_recv,
                  struct part_aos_f4_send *parts_aos_pair_f4_send,
                  struct part_aos_f4_recv *parts_aos_pair_f4_recv,
                  struct part_aos_f4_g_send *parts_aos_pair_f4_g_send,
                  struct part_aos_f4_g_recv *parts_aos_pair_f4_g_recv,
                  struct part_aos_f4_f_send *parts_aos_pair_f4_f_send,
                  struct part_aos_f4_f_recv *parts_aos_pair_f4_f_recv,
                  ticks timers_step[timer_count],
                  double timings_log_step[timer_count]);


void run_simulation(struct parameters *params);

#ifdef __cplusplus
}
#endif
