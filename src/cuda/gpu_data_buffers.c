/**
 * Functions around data buffers for gpu offloading
 */

/* CUDA Header. Wrap in extern "C" to prevent C++ function name mangling */
#ifdef __cplusplus
extern "C" {
#endif

#include "error.h"
#include "gpu_data_buffers.h"
#include "gpu_packing_defines.h"

#include <cuda.h>
#include <cuda_runtime.h>

#include <math.h>


void gpu_data_init_pack_arrays(
        struct pack_vars_self **pack_vars_self_dens,
        struct pack_vars_pair **pack_vars_pair_dens,
        struct pack_vars_self **pack_vars_self_grad,
        struct pack_vars_pair **pack_vars_pair_grad,
        struct pack_vars_self **pack_vars_self_forc,
        struct pack_vars_pair **pack_vars_pair_forc,
        const int verbose){

  cudaError_t err = cudaSuccess;

  size_t total = 0;
  if (verbose)
    message("cudaMallocHost pack_vars size=%lu, total=%lu",
        sizeof(struct pack_vars_self), 6*sizeof(struct pack_vars_self));
  total += 6*sizeof(struct pack_vars_self);

  err = cudaMallocHost((void **)pack_vars_self_dens,
                 sizeof(struct pack_vars_self));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)pack_vars_self_forc,
                 sizeof(struct pack_vars_self));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)pack_vars_self_grad,
                 sizeof(struct pack_vars_self));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)pack_vars_pair_dens,
                 sizeof(struct pack_vars_pair));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)pack_vars_pair_forc,
                 sizeof(struct pack_vars_pair));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)pack_vars_pair_grad,
                 sizeof(struct pack_vars_pair));
  swift_assert(err == cudaSuccess);

  const int target_n_tasks = TARGET_N_TASKS_PACK_SIZE;
  const int target_n_tasks_pair = TARGET_N_TASKS_PACK_SIZE_PAIR;

  (*pack_vars_self_dens)->target_n_tasks = target_n_tasks;
  (*pack_vars_pair_dens)->target_n_tasks = target_n_tasks_pair;
  (*pack_vars_self_forc)->target_n_tasks = target_n_tasks;
  (*pack_vars_pair_forc)->target_n_tasks = target_n_tasks_pair;
  (*pack_vars_self_grad)->target_n_tasks = target_n_tasks;
  (*pack_vars_pair_grad)->target_n_tasks = target_n_tasks_pair;


  // how many tasks we want in each bundle (used for launching kernels in
  // different streams)
  const int bundle_size = N_TASKS_BUNDLE_SELF;
  const int bundle_size_pair = N_TASKS_BUNDLE_PAIR;
  (*pack_vars_self_dens)->bundle_size = bundle_size;
  (*pack_vars_pair_dens)->bundle_size = bundle_size_pair;
  (*pack_vars_self_forc)->bundle_size = bundle_size;
  (*pack_vars_pair_forc)->bundle_size = bundle_size_pair;
  (*pack_vars_self_grad)->bundle_size = bundle_size;
  (*pack_vars_pair_grad)->bundle_size = bundle_size_pair;

  int nBundles = (target_n_tasks + bundle_size - 1) / bundle_size;
  int nBundles_pair =
      (target_n_tasks_pair + bundle_size_pair - 1) / bundle_size_pair;

  (*pack_vars_self_dens)->nBundles = nBundles;
  (*pack_vars_pair_dens)->nBundles = nBundles_pair;
  (*pack_vars_self_forc)->nBundles = nBundles;
  (*pack_vars_pair_forc)->nBundles = nBundles_pair;
  (*pack_vars_self_grad)->nBundles = nBundles;
  (*pack_vars_pair_grad)->nBundles = nBundles_pair;

  // first part and last part are the first and last particle ids (locally
  // within this thread). A. Nasar: All these are used in GPU offload setup

  if (verbose)
    message("cudaMallocHost pack_vars bundle_first size=%lu total=%lu nBundles=%d",
        nBundles * sizeof(int),
        27 * nBundles * sizeof(int),
        nBundles);
  total += 27 * nBundles * sizeof(int);

  err = cudaMallocHost((void **)&(*pack_vars_self_dens)->bundle_first_part,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_self_dens)->bundle_last_part,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_self_dens)->bundle_first_task_list,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)&(*pack_vars_pair_dens)->bundle_first_part,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_pair_dens)->bundle_last_part,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_pair_dens)->bundle_first_task_list,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)&(*pack_vars_self_forc)->bundle_first_part,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_self_forc)->bundle_last_part,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_self_forc)->bundle_first_task_list,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)&(*pack_vars_pair_forc)->bundle_first_part,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_pair_forc)->bundle_last_part,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_pair_forc)->bundle_first_task_list,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)&(*pack_vars_self_grad)->bundle_first_part,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_self_grad)->bundle_last_part,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_self_grad)->bundle_first_task_list,
                 nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)&(*pack_vars_pair_grad)->bundle_first_part,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_pair_grad)->bundle_last_part,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)&(*pack_vars_pair_grad)->bundle_first_task_list,
                 2 * nBundles * sizeof(int));
  swift_assert(err == cudaSuccess);


  int tasksperbundle = (target_n_tasks + nBundles - 1) / nBundles;
  int tasksperbundle_pair =
      (target_n_tasks_pair + nBundles_pair - 1) / nBundles_pair;

  (*pack_vars_self_dens)->tasksperbundle = tasksperbundle;
  (*pack_vars_pair_dens)->tasksperbundle = tasksperbundle_pair;
  (*pack_vars_self_forc)->tasksperbundle = tasksperbundle;
  (*pack_vars_pair_forc)->tasksperbundle = tasksperbundle_pair;
  (*pack_vars_self_grad)->tasksperbundle = tasksperbundle;
  (*pack_vars_pair_grad)->tasksperbundle = tasksperbundle_pair;


  (*pack_vars_self_dens)->count_parts = 0;
  (*pack_vars_pair_dens)->count_parts = 0;
  (*pack_vars_self_forc)->count_parts = 0;
  (*pack_vars_pair_forc)->count_parts = 0;
  (*pack_vars_self_grad)->count_parts = 0;
  (*pack_vars_pair_grad)->count_parts = 0;

  /* int parts_per_top_level_cell = PARTS_PER_TOP_LEVEL_CELL; */
      /* space->nr_local_cells_with_particles / */
      /* space->nr_parts; */
      /*A. Nasar: What I think is a good approximation for */
         /*average N particles in each top level cell*/
  /* float eta_neighbours = e->s->eta_neighbours; */
  /* int np_per_cell = ceil(2.0 * eta_neighbours); */
  /* np_per_cell *= np_per_cell * np_per_cell; */
  int np_per_cell = PARTS_PER_CELL;
  /*A. Nasar: Increase parts per recursed task-level cell by buffer to
    ensure we allocate enough memory*/
  int buff = ceil(0.5 * np_per_cell);
  /*A. Nasar: Multiplication by 2 is also to ensure we do not over-run
   *  the allocated memory on buffers and GPU. This can happen if calculated h
   * is larger than cell width and splitting makes bigger than target cells*/
  int count_max_parts_tmp = 64 * 8 * target_n_tasks * (np_per_cell + buff);

  (*pack_vars_self_dens)->count_max_parts = count_max_parts_tmp;
  (*pack_vars_pair_dens)->count_max_parts = count_max_parts_tmp;
  (*pack_vars_self_forc)->count_max_parts = count_max_parts_tmp;
  (*pack_vars_pair_forc)->count_max_parts = count_max_parts_tmp;
  (*pack_vars_self_grad)->count_max_parts = count_max_parts_tmp;
  (*pack_vars_pair_grad)->count_max_parts = count_max_parts_tmp;


  if (verbose)
    message("Calloc dens self task_list size1=%lu size2=%lu target_n_tasks %d",
        target_n_tasks * sizeof(struct task*),
        target_n_tasks * sizeof(struct cell*),
        target_n_tasks);
  total += target_n_tasks * sizeof(struct task*) + target_n_tasks * sizeof(struct cell*);

  // a list of the cells and tasks the GPU will work on
  (*pack_vars_self_dens)->task_list =
      (struct task **)calloc(target_n_tasks, sizeof(struct task *));
  (*pack_vars_self_dens)->cell_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));

  if (verbose)
    message("Calloc dens pair task_list size1=%lu size2=%lu target_n_tasks %d",
        target_n_tasks * sizeof(struct task*),
        target_n_tasks * sizeof(struct cell*),
        target_n_tasks);
  total += target_n_tasks * sizeof(struct task*) + target_n_tasks * sizeof(struct cell*);
  (*pack_vars_pair_dens)->task_list =
      (struct task **)calloc(target_n_tasks, sizeof(struct task *));
  (*pack_vars_pair_dens)->top_task_list =
      (struct task **)calloc(target_n_tasks, sizeof(struct task *));
  int n_leaves_max = 4096;

  /*Allocate target_n_tasks for top level tasks. This is a 2D array with length target_n_tasks and width n_leaves_max*/
  (*pack_vars_pair_dens)->leaf_list = (struct leaf_cell_list *)calloc(target_n_tasks, sizeof(struct leaf_cell_list));

  if (verbose)
    message("Malloc dens pair leaf_list size1=%lu size2=%lu total=%lu target_n_tasks %d n_leaves_max %d",
        n_leaves_max * sizeof(struct cell*),
        n_leaves_max * sizeof(double),
        target_n_tasks * (2 * n_leaves_max * sizeof(struct cell*) + 3 * n_leaves_max * sizeof(double)),
        target_n_tasks, n_leaves_max);
  total += target_n_tasks * (2 * n_leaves_max * sizeof(struct cell*) + 3 * n_leaves_max * sizeof(double));

  for (int i = 0; i < target_n_tasks; i++){
    (*pack_vars_pair_dens)->leaf_list[i].ci = (struct cell**)malloc(n_leaves_max * sizeof(struct cell *));
    (*pack_vars_pair_dens)->leaf_list[i].cj = (struct cell**)malloc(n_leaves_max * sizeof(struct cell *));
    (*pack_vars_pair_dens)->leaf_list[i].shiftx = (double*)malloc(n_leaves_max * sizeof(double));
    (*pack_vars_pair_dens)->leaf_list[i].shifty = (double*)malloc(n_leaves_max * sizeof(double));
    (*pack_vars_pair_dens)->leaf_list[i].shiftz = (double*)malloc(n_leaves_max * sizeof(double));
    (*pack_vars_pair_dens)->leaf_list[i].n_leaves = 0;
    (*pack_vars_pair_dens)->leaf_list[i].n_packed = 0;
    (*pack_vars_pair_dens)->leaf_list[i].n_offload = 0;
  }
  /*Allocate memory for n_leaves_max task pointers per top level task*/

  if (verbose)
    message("Calloc dens pair cell lists size=%lu total=%lu target_n_tasks=%d",
        target_n_tasks * sizeof(struct cell *),
        2 * target_n_tasks * sizeof(struct cell *),
        target_n_tasks);
  total += 2 * target_n_tasks * sizeof(struct cell *),
  (*pack_vars_pair_dens)->ci_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));
  (*pack_vars_pair_dens)->cj_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));

  if (verbose)
    message("Calloc force self cell lists size1=%lu size2=%lu total=%lu target_n_tasks=%d",
        target_n_tasks * sizeof(struct task *),
        target_n_tasks * sizeof(struct cell *),
        target_n_tasks * sizeof(struct task *) + target_n_tasks * sizeof(struct cell *),
        target_n_tasks);
  total += target_n_tasks * sizeof(struct task *) + target_n_tasks * sizeof(struct cell *);

  (*pack_vars_self_forc)->task_list =
      (struct task **)calloc(target_n_tasks, sizeof(struct task *));
  (*pack_vars_self_forc)->cell_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));

  if (verbose)
    message("Calloc force pair cell lists size1=%lu size2=%lu total=%lu target_n_tasks=%d",
        target_n_tasks * sizeof(struct task *),
        target_n_tasks * sizeof(struct cell *),
        target_n_tasks * sizeof(struct task *) + 2 * target_n_tasks * sizeof(struct cell *),
        target_n_tasks);
  total += target_n_tasks * sizeof(struct task *) + 2 * target_n_tasks * sizeof(struct cell *);

  (*pack_vars_pair_forc)->task_list =
      (struct task **)calloc(target_n_tasks, sizeof(struct task *));
  (*pack_vars_pair_forc)->ci_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));
  (*pack_vars_pair_forc)->cj_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));

  if (verbose)
    message("Calloc grad self cell lists size1=%lu size2=%lu total=%lu target_n_tasks=%d",
        target_n_tasks * sizeof(struct task *),
        target_n_tasks * sizeof(struct cell *),
        target_n_tasks * sizeof(struct task *) + target_n_tasks * sizeof(struct cell *),
        target_n_tasks);
  total += target_n_tasks * sizeof(struct task *) + target_n_tasks * sizeof(struct cell *);

  (*pack_vars_self_grad)->task_list =
      (struct task **)calloc(target_n_tasks, sizeof(struct task *));
  (*pack_vars_self_grad)->cell_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));

  if (verbose)
    message("Calloc grad pair cell lists size1=%lu size2=%lu total=%lu target_n_tasks=%d",
        target_n_tasks * sizeof(struct task *),
        target_n_tasks * sizeof(struct cell *),
        target_n_tasks * sizeof(struct task *) + 2 * target_n_tasks * sizeof(struct cell *),
        target_n_tasks);
  total += target_n_tasks * sizeof(struct task *) + 2 * target_n_tasks * sizeof(struct cell *);

  (*pack_vars_pair_grad)->task_list =
      (struct task **)calloc(target_n_tasks, sizeof(struct task *));
  (*pack_vars_pair_grad)->ci_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));
  (*pack_vars_pair_grad)->cj_list =
      (struct cell **)calloc(target_n_tasks, sizeof(struct cell *));

  if(verbose)
    message("Total alloc'd memory: %lu", total);

}



void gpu_data_init_first_part_host_arrays(
    int2 **task_first_part_f4,
    int2 **task_first_part_f4_g,
    int2 **task_first_part_f4_f,
    /* int2 **d_task_first_part_f4, */
    /* int2 **d_task_first_part_f4_g, */
    /* int2 **d_task_first_part_f4_f */
    const int verbose
        ){

  const int target_n_tasks = TARGET_N_TASKS_PACK_SIZE;
  /* const int target_n_tasks_pair = TARGET_N_TASKS_PACK_SIZE_PAIR; */

  cudaError_t err = cudaSuccess;

  if (verbose)
    message("cudaMallocHost first_part arrays size=%lu total = %lu target_n_tasks = %d",
      target_n_tasks * sizeof(int2),
      3 * target_n_tasks * sizeof(int2),
      target_n_tasks
      );

  err = cudaMallocHost((void **)task_first_part_f4, target_n_tasks * sizeof(int2));
  swift_assert(err == cudaSuccess);

  /* err = cudaMalloc((void **)d_task_first_part_f4, target_n_tasks * sizeof(int2)); */
  /* swift_assert(err == cudaSuccess); */

  err = cudaMallocHost((void **)task_first_part_f4_f, target_n_tasks * sizeof(int2));
  swift_assert(err == cudaSuccess);

  /* err = cudaMalloc((void **)d_task_first_part_f4_f, target_n_tasks * sizeof(int2)); */
  /* swift_assert(err == cudaSuccess); */

  err = cudaMallocHost((void **)task_first_part_f4_g, target_n_tasks * sizeof(int2));
  swift_assert(err == cudaSuccess);

  /* err = cudaMalloc((void **)d_task_first_part_f4_g, target_n_tasks * sizeof(int2)); */
  /* swift_assert(err == cudaSuccess); */

}


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
  const int count_max_parts_tmp,
  const int verbose
    ){

  cudaError_t err = cudaSuccess;

  if (verbose)
    message("cudaMallocHost parts_aos_f4 size=%lu total=%lu count_max_parts_tmp=%d",
                 count_max_parts_tmp * sizeof(struct part_aos_f4_send),
                 18 * count_max_parts_tmp * sizeof(struct part_aos_f4_send),
                 count_max_parts_tmp
        );

  /*Now allocate memory for Buffer and GPU particle arrays*/
  err = cudaMallocHost((void **)parts_aos_f4_send,
                 count_max_parts_tmp * sizeof(struct part_aos_f4_send));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)parts_aos_f4_recv,
                 count_max_parts_tmp * sizeof(struct part_aos_f4_recv));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)parts_aos_forc_f4_send,
                 count_max_parts_tmp * sizeof(struct part_aos_f4_f_send));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)parts_aos_forc_f4_recv,
                 count_max_parts_tmp * sizeof(struct part_aos_f4_f_recv));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)parts_aos_grad_f4_send,
                 count_max_parts_tmp * sizeof(struct part_aos_f4_g_send));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)parts_aos_grad_f4_recv,
                 count_max_parts_tmp * sizeof(struct part_aos_f4_g_recv));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)parts_aos_pair_f4_send,
                 2 * count_max_parts_tmp * sizeof(struct part_aos_f4_send));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)parts_aos_pair_f4_recv,
                 2 * count_max_parts_tmp * sizeof(struct part_aos_f4_recv));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)parts_aos_pair_f4_g_send,
                 2 * count_max_parts_tmp * sizeof(struct part_aos_f4_g_send));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)parts_aos_pair_f4_g_recv,
                 2 * count_max_parts_tmp * sizeof(struct part_aos_f4_g_recv));
  swift_assert(err == cudaSuccess);

  err = cudaMallocHost((void **)parts_aos_pair_f4_f_send,
                 2 * count_max_parts_tmp * sizeof(struct part_aos_f4_f_send));
  swift_assert(err == cudaSuccess);
  err = cudaMallocHost((void **)parts_aos_pair_f4_f_recv,
                 2 * count_max_parts_tmp * sizeof(struct part_aos_f4_f_recv));
  swift_assert(err == cudaSuccess);

}



void gpu_data_clear_pack_arrays(
    struct pack_vars_self **pack_vars_self_dens,
    struct pack_vars_pair **pack_vars_pair_dens,
    struct pack_vars_self **pack_vars_self_grad,
    struct pack_vars_pair **pack_vars_pair_grad,
    struct pack_vars_self **pack_vars_self_forc,
    struct pack_vars_pair **pack_vars_pair_forc){

  cudaError_t err = cudaSuccess;

  err = cudaFreeHost((void *)(*pack_vars_self_dens));
  swift_assert(err == cudaSuccess);

  /* err = cudaFreeHost((void *)(*pack_vars_pair_dens)); */
  /* swift_assert(err == cudaSuccess); */
  /* err = cudaFreeHost((void *)(*pack_vars_self_grad)); */
  /* swift_assert(err == cudaSuccess); */
  /* err = cudaFreeHost((void *)(*pack_vars_pair_grad)); */
  /* swift_assert(err == cudaSuccess); */
  /* err = cudaFreeHost((void *)(*pack_vars_self_forc)); */
  /* swift_assert(err == cudaSuccess); */
  /* err = cudaFreeHost((void *)(*pack_vars_pair_forc)); */
  /* swift_assert(err == cudaSuccess); */
}


void test_alloc_and_free(int** test2){


  printf("Check0\n");
  fflush(stdout);

  int *test_int_p = NULL;
  cudaError_t err = cudaSuccess;
  err = cudaMallocHost((void **)&test_int_p, sizeof(int));
  printf("Check0.1\n");
  fflush(stdout);

  struct pack_vars_self *test_p = NULL;
  /* cudaError_t err = cudaSuccess; */
  err = cudaMallocHost((void **)&test_p,
                 sizeof(struct pack_vars_self));
  printf("Check1\n");
  fflush(stdout);

  err = cudaMallocHost((void **)test2,
                 10*sizeof(int));
  printf("Check2\n");
  fflush(stdout);

  err = cudaFreeHost((void*) test_p);
  swift_assert(err == cudaSuccess);
  printf("Check3\n");
  fflush(stdout);

  err = cudaFreeHost((void*) *test2);
  swift_assert(err == cudaSuccess);
  printf("Check4\n");
  fflush(stdout);

  printf("Test success\n");
  fflush(stdout);
}



void gpu_data_clear_first_part_host_arrays(
    int2 **task_first_part_f4,
    int2 **task_first_part_f4_g,
    int2 **task_first_part_f4_f ) {
    /* int2 **d_task_first_part_f4, */
    /* int2 **d_task_first_part_f4_g, */
    /* int2 **d_task_first_part_f4_f) */
  error("TODO");
}


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
    ){
  error("TODO");
}


#ifdef __cplusplus
}
#endif

