/* This object's header. */
#include "runner_gpu_pack_functions.h"

/* Local headers. */
#include "cell.h"
#include "hydro_part.h"

#include <string.h>


void runner_doself1_gpu_pack_neat_aos_f4(
    struct runner *r, struct cell *__restrict__ c,
    struct part_aos_f4_send *__restrict__ parts_aos_buffer, int timer,
    int *pack_length, int tid, int count_max_parts_tmp) {

  /* TIMER_TIC; */

  /* Anything to do here? */
  if (c->hydro.count == 0) return;

  int count = c->hydro.count;
  int local_pack_position = (*pack_length);

#ifdef SWIFT_DEBUG_CHECKS
  if (local_pack_position + count >= count_max_parts_tmp) {
    fprintf(stderr,
            "Exceeded count_max_parts_tmp. Make arrays bigger! count_max %i "
            "count %i\n",
            count_max_parts_tmp, local_pack_position + count);
    error("0");
  }
#endif
  int2 frst_lst_prts = {local_pack_position, local_pack_position + count};
  /* Pack the particle data into CPU-side buffers*/
  pack_neat_aos_f4(c, parts_aos_buffer, tid, local_pack_position, count,
                   frst_lst_prts);
  /* Increment pack length accordingly */
  (*pack_length) += count;

  /* if (timer) TIMER_TOC(timer_doself_gpu_pack); */
}


void pack_neat_aos_f4(struct cell *__restrict__ c,
                      struct part_aos_f4_send *__restrict__ parts_aos_buffer,
                      int tid, int local_pack_position, int count,
                      int2 frst_lst_prts) {

  struct part ptmps[count];
  memcpy(ptmps, (c->hydro.parts), count * sizeof(struct part));
  //  ptmps = c->hydro.parts;
  const float cellx = c->loc[0];
  const float celly = c->loc[1];
  const float cellz = c->loc[2];
  for (int i = 0; i < count; i++) {
    const int id_in_pack = i + local_pack_position;
    //    const struct part p = ptmps[i];
    /*Data to be copied to GPU*/
    const double *x = part_get_const_x(&ptmps[i]);
    parts_aos_buffer[id_in_pack].x_p_h.x = x[0] - cellx;
    parts_aos_buffer[id_in_pack].x_p_h.y = x[1] - celly;
    parts_aos_buffer[id_in_pack].x_p_h.z = x[2] - cellz;
    parts_aos_buffer[id_in_pack].x_p_h.w = part_get_h(&ptmps[i]);
    const float *v = part_get_const_v(&ptmps[i]);
    parts_aos_buffer[id_in_pack].ux_m.x = v[0];
    parts_aos_buffer[id_in_pack].ux_m.y = v[1];
    parts_aos_buffer[id_in_pack].ux_m.z = v[2];
    parts_aos_buffer[id_in_pack].ux_m.w = part_get_mass(&ptmps[i]);
    //    /*Initialise sums to zero before CPU/GPU copy*/
    //    const float4 zeroes = {0.0, 0.0, 0.0, 0.0};
    //    parts_aos_buffer[id_in_pack].rho_dh_wcount = zeroes;
    //    parts_aos_buffer[id_in_pack].rot_ux_div_v = zeroes;
  }
}
