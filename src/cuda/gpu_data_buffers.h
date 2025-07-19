#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <vector_types.h>

#include "pack_vars.h"
#include "part_gpu.h"

void gpu_data_init_pack_arrays(
    struct pack_vars_self **pack_vars_self_dens,
    struct pack_vars_pair **pack_vars_pair_dens,
    struct pack_vars_self **pack_vars_self_grad,
    struct pack_vars_pair **pack_vars_pair_grad,
    struct pack_vars_self **pack_vars_self_forc,
    struct pack_vars_pair **pack_vars_pair_forc);


void gpu_data_init_first_part_host_arrays(
    int2 **task_first_part_f4,
    int2 **task_first_part_f4_g,
    int2 **task_first_part_f4_f
    /* int2 **d_task_first_part_f4, */
    /* int2 **d_task_first_part_f4_g, */
    /* int2 **d_task_first_part_f4_f */
        );

/**
 * TODO: restructure to one function call per array
 */
void gpu_data_init_send_recv_host_arrays(
  struct part_aos_f4_send **parts_aos_f4_send,
  struct part_aos_f4_recv **parts_aos_f4_recv,
  struct part_aos_f4_g_send **parts_aos_grad_f4_send,
  struct part_aos_f4_g_recv **parts_aos_grad_f4_recv,
  struct part_aos_f4_f_send **parts_aos_forc_f4_send,
  struct part_aos_f4_f_recv **parts_aos_forc_f4_recv,
  struct part_aos_f4_send **parts_aos_pair_f4_send,
  struct part_aos_f4_recv **parts_aos_pair_f4_recv,
  struct part_aos_f4_g_send **parts_aos_pair_f4_g_send,
  struct part_aos_f4_g_recv **parts_aos_pair_f4_g_recv,
  struct part_aos_f4_f_send **parts_aos_pair_f4_f_send,
  struct part_aos_f4_f_recv **parts_aos_pair_f4_f_recv,
  const int count_max_parts_tmp
    );





void gpu_data_clear_pack_arrays(
    struct pack_vars_self **pack_vars_self_dens,
    struct pack_vars_pair **pack_vars_pair_dens,
    struct pack_vars_self **pack_vars_self_grad,
    struct pack_vars_pair **pack_vars_pair_grad,
    struct pack_vars_self **pack_vars_self_forc,
    struct pack_vars_pair **pack_vars_pair_forc);


void gpu_data_clear_first_part_host_arrays(
    int2 **task_first_part_f4,
    int2 **task_first_part_f4_g,
    int2 **task_first_part_f4_f
    /* int2 **d_task_first_part_f4, */
    /* int2 **d_task_first_part_f4_g, */
    /* int2 **d_task_first_part_f4_f */
    );


/**
 * TODO: restructure to one function call per array
 */
void gpu_data_clear_send_recv_host_arrays(
  struct part_aos_f4_send **parts_aos_f4_send,
  struct part_aos_f4_recv **parts_aos_f4_recv,
  struct part_aos_f4_g_send **parts_aos_grad_f4_send,
  struct part_aos_f4_g_recv **parts_aos_grad_f4_recv,
  struct part_aos_f4_f_send **parts_aos_forc_f4_send,
  struct part_aos_f4_f_recv **parts_aos_forc_f4_recv,
  struct part_aos_f4_send **parts_aos_pair_f4_send,
  struct part_aos_f4_recv **parts_aos_pair_f4_recv,
  struct part_aos_f4_g_send **parts_aos_pair_f4_g_send,
  struct part_aos_f4_g_recv **parts_aos_pair_f4_g_recv,
  struct part_aos_f4_f_send **parts_aos_pair_f4_f_send,
  struct part_aos_f4_f_recv **parts_aos_pair_f4_f_recv
    );


#ifdef __cplusplus
}
#endif
