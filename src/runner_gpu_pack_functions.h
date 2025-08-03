#pragma once

#include "config.h"
#include "runner.h"
#include "timeline.h"


#ifdef HAVE_CUDA
#include "cuda/part_gpu.h"
#endif


void runner_doself1_gpu_pack_neat_aos_f4(
    struct runner *r, struct cell *__restrict__ c,
    struct part_aos_f4_send *__restrict__ parts_aos, int timer,
    int *pack_length, int tid, int count_max_parts_tmp);

void pack_neat_aos_f4(struct cell *__restrict__ c,
                      struct part_aos_f4_send *__restrict__ parts_aos_buffer,
                      int tid, int local_pack_position, int count,
                      int2 frst_lst_prts);
