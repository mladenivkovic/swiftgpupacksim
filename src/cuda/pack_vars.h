#pragma once

struct pack_vars_self {
  /*List of tasks and respective cells to be packed*/
  struct task **task_list;
  struct task **top_task_list;
  struct cell **cell_list;
  /*List of cell positions*/
  double *cellx;
  double *celly;
  double *cellz;
  /*List of cell positions*/
  double *d_cellx;
  double *d_celly;
  double *d_cellz;
  int bundle_size;
  /*How many particles in a bundle*/
  int count_parts;
  /**/
  int tasks_packed;
  int top_tasks_packed;
  int *task_first_part;
  int *task_last_part;
  int *d_task_first_part;
  int *d_task_last_part;
  int *bundle_first_part;
  int *bundle_last_part;
  int *bundle_first_task_list;
  int count_max_parts;
  int launch;
  int launch_leftovers;
  int target_n_tasks;
  int nBundles;
  int tasksperbundle;
};

struct leaf_cell_list {
  struct cell **ci;
  struct cell **cj;
  double *shiftx;
  double *shifty;
  double *shiftz;
  int n_leaves;
  int n_packed;
  int citop;
  int cjtop;
  int lpdt;  // lpdt is an acronym for "last packed daughter task" before we
             // launched on GPU
  int n_end;
  int n_offload;
};

struct pack_vars_pair {
  /*List of tasks and respective cells to be packed*/
  struct task **task_list;
  struct task **top_task_list;
  struct leaf_cell_list *leaf_list;
  struct cell **ci_list;
  struct cell **cj_list;
  /*List of cell shifts*/
  double *shiftx;
  double *shifty;
  double *shiftz;
  /*List of cell shifts*/
  double *d_shiftx;
  double *d_shifty;
  double *d_shiftz;
  int bundle_size;
  /*How many particles in a bundle*/
  int count_parts;
  /**/
  int tasks_packed;
  int top_tasks_packed;
  int *task_first_part;
  int *task_last_part;
  int *d_task_first_part;
  int *d_task_last_part;
  int *bundle_first_part;
  int *bundle_last_part;
  int *bundle_first_task_list;
  int count_max_parts;
  int launch;
  int launch_leftovers;
  int target_n_tasks;
  int nBundles;
  int tasksperbundle;
  int task_locked;

};


