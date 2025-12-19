#pragma once


#include "lock.h"
#include "timeline.h"


/**
 * @brief BHs-related cell variables.
 */
struct cell_black_holes {

  /* If we are not using BHs, compact as much of the unecessary variables
     into an anonymous union to save memory in the cell structure. */
#ifdef BLACK_HOLES_NONE
  union {
#endif

    /*! Pointer to the #bpart data. */
    struct bpart* parts;

    /*! The drift task for bparts */
    struct task* drift;

    /*! Implicit tasks marking the entry of the BH physics block of tasks */
    struct task* black_holes_in;

    /*! Implicit tasks marking the exit of the BH physics block of tasks */
    struct task* black_holes_out;

    /*! The black hole ghost task itself */
    struct task* density_ghost;

    /*! The ghost tasks related to BH swallowing */
    struct task* swallow_ghost_1;
    struct task* swallow_ghost_2;
    struct task* swallow_ghost_3;

    /*! Linked list of the tasks computing this cell's BH density. */
    struct link* density;

    /*! Linked list of the tasks computing this cell's BH swallowing and
     * merging. */
    struct link* swallow;

    /*! Linked list of the tasks processing the particles to swallow */
    struct link* do_gas_swallow;

    /*! Linked list of the tasks processing the particles to swallow */
    struct link* do_bh_swallow;

    /*! Linked list of the tasks computing this cell's BH feedback. */
    struct link* feedback;

    /*! Last (integer) time the cell's bpart were drifted forward in time. */
    integertime_t ti_old_part;

    /*! Nr of #bpart this cell can hold after addition of new #bpart. */
    int count_total;

    /*! Max smoothing length of active particles in this cell. */
    float h_max_active;

    /*! Values of h_max before the drifts, used for sub-cell tasks. */
    float h_max_old;

    /*! Maximum part movement in this cell since last construction. */
    float dx_max_part;

    /*! Values of dx_max before the drifts, used for sub-cell tasks. */
    float dx_max_part_old;

#ifdef BLACK_HOLES_NONE
  };
#endif

  /*! Maximum end of (integer) time step in this cell for black tasks. */
  integertime_t ti_end_min;

  /*! Maximum beginning of (integer) time step in this cell for black hole
   * tasks. */
  integertime_t ti_beg_max;

  /*! Spin lock for various uses (#bpart case). */
  swift_lock_type lock;

  /*! Max smoothing length in this cell. */
  float h_max;

  /*! Is the #bpart data of this cell being used in a sub-cell? */
  int hold;

  /*! Number of #bpart updated in this cell. */
  int updated;

  /*! Nr of #bpart in this cell. */
  int count;
};


/**
 * @brief Gravity-related cell variables.
 */

struct cell_grav {

  union {

    /*! Pointer to the #gpart data. */
    struct gpart* parts;

    /*! or #gpart_foreign data. */
    struct gpart_foreign* parts_foreign;

    /*! or #gpart_fof_foreign data. */
    struct gpart_fof_foreign* parts_fof_foreign;
  };

  union {

    /*! Pointer to the #gpart data at rebuild time. */
    struct gpart* parts_rebuild;

    /*! Pointer to the #gpart_foreign data at rebuild time. */
    struct gpart_foreign* parts_foreign_rebuild;

    /*! Pointer to the #gpart_fof_foreign data at rebuild time. */
    struct gpart_fof_foreign* parts_fof_foreign_rebuild;
  };

  /*! This cell's multipole. */
  struct gravity_tensors* multipole;

  /*! Super cell, i.e. the highest-level parent cell that has a grav pair/self
   * tasks */
  struct cell* super;

  /*! The drift task for gparts */
  struct task* drift;

  /*! Implicit task (going up- and down the tree) for the #gpart drifts */
  struct task* drift_out;

  /*! Linked list of the tasks computing this cell's gravity forces. */
  struct link* grav;

  /*! Linked list of the tasks computing this cell's gravity M-M forces. */
  struct link* mm;

  /*! The multipole initialistation task */
  struct task* init;

  /*! Implicit task for the gravity initialisation */
  struct task* init_out;

  /*! Task computing long range non-periodic gravity interactions */
  struct task* long_range;

  /*! Implicit task for the down propagation */
  struct task* down_in;

  /*! Task propagating the multipole to the particles */
  struct task* down;

  /*! The task to end the force calculation */
  struct task* end_force;

  /*! Task for weighting neutrino particles */
  struct task* neutrino_weight;

  /*! Minimum end of (integer) time step in this cell for gravity tasks. */
  integertime_t ti_end_min;

  /*! Maximum beginning of (integer) time step in this cell for gravity tasks.
   */
  integertime_t ti_beg_max;

  /*! Last (integer) time the cell's gpart were drifted forward in time. */
  integertime_t ti_old_part;

  /*! Last (integer) time the cell's multipole was drifted forward in time. */
  integertime_t ti_old_multipole;

  /*! Spin lock for various uses (#gpart case). */
  swift_lock_type plock;

  /*! Spin lock for various uses (#multipole case). */
  swift_lock_type mlock;

  /*! Spin lock for star formation use. */
  swift_lock_type star_formation_lock;

  /*! Nr of #gpart in this cell. */
  int count;

  /*! Nr of #gpart this cell can hold after addition of new #gpart. */
  int count_total;

  /*! Number of #gpart updated in this cell. */
  int updated;

  /*! Is the #gpart data of this cell being used in a sub-cell? */
  int phold;

  /*! Is the #multipole data of this cell being used in a sub-cell? */
  int mhold;

  /*! Number of M-M tasks that are associated with this cell. */
  short int nr_mm_tasks;
};

/*! @brief Enum indicating the completeness for the Voronoi mesh of this cell.
 *
 * A cell is considered complete when it and its neighbours on the same level in
 * the AMR have at least one particle in every 1/27th cube of the cell (obtained
 * by dividing cells in three along all axes).
 *
 * The Voronoi grid can safely be constructed on any level where the cell is
 * complete. */
enum grid_completeness {
  grid_invalidated_completeness = 0,
  grid_complete,
  grid_incomplete,
};


struct cell_grid {
  /*! Pointer to parent where the grid is constructed. */
  struct cell* construction_level;

  /*! Pointer to shallowest parent of this cell used in any pair construction
   * task. Can be above the construction level of this cell. We need to drift at
   * this level. */
  struct cell* super;

  /*! Whether this cell is complete (at least one particle in all 27 sub-cells
   * if this cell is divided in thirds along each axis). */
  enum grid_completeness self_completeness;

  /*! Whether this cell is itself complete and has directly neighbouring cell
   * on the same level in all directions which are also complete. */
  int complete;

#ifdef WITH_MPI
  /*! Flags indicating whether we should send the faces for the corresponding
   * SIDs over MPI */
  int send_flags;
#endif

  /*! Pointer to the voronoi struct of this cell (if any) */
  struct voronoi* voronoi;

  /*! Pointer to this cells construction task. */
  struct task* construction;

  /*! Linked list of this cells outgoing construction synchronization tasks
   * (i.e. for cells that need this cell for their construction task) */
  struct link* sync_out;

  /*! Linked list of this cells incoming construction synchronization tasks
   * (i.e. cells needed for this cell's construction task) */
  struct link* sync_in;

  /*! Time of last construction */
  integertime_t ti_old;
};


/**
 * @brief Sinks-related cell variables.
 */
struct cell_sinks {

  /* If we are not using sinks, compact as much of the unecessary variables
     into an anonymous union to save memory in the cell structure. */
#ifdef SINK_NONE
  union {
#endif

    /*! Pointer to the #sink data. */
    struct sink* parts;

    /*! Pointer to the #spart data at rebuild time. */
    struct sink* parts_rebuild;

    /*! Linked list of the tasks computing this cell's sink swallow. */
    struct link* swallow;

    /*! Linked list of the tasks computing this cell's sink do_gas_swallow. */
    struct link* do_gas_swallow;

    /*! Linked list of the tasks computing this cell's sink do_sink_swallow. */
    struct link* do_sink_swallow;

    /*! The drift task for sinks */
    struct task* drift;

    /*! Implicit tasks marking the entry of the sink block of tasks */
    struct task* sink_in;

    /*! The sink ghost task itself */
    struct task* density_ghost;

    /*! Linked list of the tasks computing this cell's sink density. */
    struct link* density;

    /*! Implicit tasks marking the end of sink swallow */
    struct task* sink_ghost1;

    /*! Implicit tasks marking the separation between do_gas_swallow and
     * do_sink_swallow */
    struct task* sink_ghost2;

    /*! Implicit tasks marking the exit of the sink block of tasks */
    struct task* sink_out;

    /*! Task for star formation from sink particles */
    struct task* star_formation_sink;

    /*! Task for sink formation */
    struct task* sink_formation;

    /*! Last (integer) time the cell's sink were drifted forward in time. */
    integertime_t ti_old_part;

    /*! Spin lock for sink formation use. */
    swift_lock_type sink_formation_lock;

    /*! Nr of #sink this cell can hold after addition of new one. */
    int count_total;

    /*! Max smoothing length of active particles in this cell.
     */
    float h_max_active;

    /*! Values of h_max before the drifts, used for sub-cell tasks. */
    float h_max_old;

    /*! Maximum part movement in this cell since last construction. */
    float dx_max_part;

    /*! Values of dx_max before the drifts, used for sub-cell tasks. */
    float dx_max_part_old;

#ifdef SINK_NONE
  };
#endif

  /*! Minimum end of (integer) time step in this cell for sink tasks. */
  integertime_t ti_end_min;

  /*! Maximum beginning of (integer) time step in this cell for sink
   * tasks. */
  integertime_t ti_beg_max;

  /*! Spin lock for various uses (#sink case). */
  swift_lock_type lock;

  /*! Max smoothing length in this cell. */
  float h_max;

  /*! Number of #sink updated in this cell. */
  int updated;

  /*! Is the #sink data of this cell being used in a sub-cell? */
  int hold;

  /*! Nr of #sink in this cell. */
  int count;
};


/**
 * @brief Stars-related cell variables.
 */
struct cell_stars {

  /* If we are not using stars, compact as much of the unecessary variables
     into an anonymous union to save memory in the cell structure. */
#ifdef STARS_NONE
  union {
#endif

    /*! Pointer to the #spart data. */
    struct spart* parts;

    /*! Pointer to the #spart data at rebuild time. */
    struct spart* parts_rebuild;

    /*! The star ghost task itself */
    struct task* density_ghost;

    /*! The first star ghost task related to kinetic feedback */
    struct task* prep1_ghost;

    /*! The second star ghost task related to kinetic feedback */
    struct task* prep2_ghost;

    /*! Linked list of the tasks computing this cell's star density. */
    struct link* density;

    /*! Linked list of the tasks computing this cell's star 1st prep for kinetic
     * feedback. */
    struct link* prepare1;

    /*! Linked list of the tasks computing this cell's star 2nd prep for kinetic
     * feedback. */
    struct link* prepare2;

    /*! Linked list of the tasks computing this cell's star feedback. */
    struct link* feedback;

    /*! The task computing this cell's sorts before the density. */
    struct task* sorts;

    /*! The drift task for sparts */
    struct task* drift;

    /*! Implicit tasks marking the entry of the stellar physics block of tasks
     */
    struct task* stars_in;

    /*! Implicit tasks marking the exit of the stellar physics block of tasks */
    struct task* stars_out;

    /*! Pointer for the sorted indices. */
    struct sort_entry* sort;

    /*! Last (integer) time the cell's spart were drifted forward in time. */
    integertime_t ti_old_part;

    /*! Spin lock for star formation use. */
    swift_lock_type star_formation_lock;

    /*! Nr of #spart this cell can hold after addition of new #spart. */
    int count_total;

    /*! Max smoothing length of active particles in this cell. */
    float h_max_active;

    /*! Values of h_max before the drifts, used for sub-cell tasks. */
    float h_max_old;

    /*! Maximum part movement in this cell since last construction. */
    float dx_max_part;

    /*! Values of dx_max before the drifts, used for sub-cell tasks. */
    float dx_max_part_old;

    /*! Maximum particle movement in this cell since the last sort. */
    float dx_max_sort;

    /*! Values of dx_max_sort before the drifts, used for sub-cell tasks. */
    float dx_max_sort_old;

    /*! Bit mask of sort directions that will be needed in the next timestep. */
    uint16_t requires_sorts;

    /*! Bit-mask indicating the sorted directions */
    uint16_t sorted;

    /*! Bit-mask indicating the sorted directions */
    uint16_t sort_allocated;

    /*! Bit mask of sorts that need to be computed for this cell. */
    uint16_t do_sort;

    /*! Star formation history struct */
    /* struct star_formation_history sfh; */

#ifdef SWIFT_DEBUG_CHECKS
    /*! Last (integer) time the cell's sort arrays were updated. */
    integertime_t ti_sort;
#endif

#ifdef STARS_NONE
  };
#endif

  /*! Maximum end of (integer) time step in this cell for star tasks. */
  integertime_t ti_end_min;

  /*! Maximum beginning of (integer) time step in this cell for star tasks.
   */
  integertime_t ti_beg_max;

  /*! Spin lock for various uses (#spart case). */
  swift_lock_type lock;

  /*! Max smoothing length in this cell. */
  float h_max;

  /*! Number of #spart updated in this cell. */
  int updated;

  /*! Nr of #spart in this cell. */
  int count;

  /*! Is the #spart data of this cell being used in a sub-cell? */
  int hold;
};

/**
 * @brief Radiative transfer related cell variables.
 */
struct cell_rt {

  /* If we are not using RT, compact as much of the unecessary variables
     into an anonymous union to save memory in the cell structure. */
#ifdef RT_NONE
  union {
#endif

    /*! Radiative transfer ghost in task */
    struct task* rt_in;

    /*! Radiative transfer ghost1 task (finishes up injection) */
    struct task* rt_ghost1;

    /*! Task for self/pair gradient step of radiative transfer */
    struct link* rt_gradient;

    /*! Radiative transfer ghost2 task */
    struct task* rt_ghost2;

    /*! Task for self/pair transport step of radiative transfer */
    struct link* rt_transport;

    /*! Radiative transfer transport out task */
    struct task* rt_transport_out;

    /*! Radiative transfer thermochemistry task */
    struct task* rt_tchem;

    /*! Radiative transfer cell time advancement task */
    struct task* rt_advance_cell_time;

    /*! Sort a cell after a recv rt gradients */
    struct task* rt_sorts;

    /*! Collect the cell times from the super to the top level */
    struct task* rt_collect_times;

    /*! Radiative transfer ghost out task */
    struct task* rt_out;

    /*! Bit mask of sorts that need to be computed for this cell.
     * Needed to be able to skip sorting undrifted cells. */
    uint16_t do_sort;

#ifdef RT_NONE
  };
#endif

#ifdef SWIFT_RT_DEBUG_CHECKS
  /*! has rt_advance_cell_time run on this cell? */
  int advanced_time;
#endif

  /*! Minimum end of (integer) time step in this cell for RT tasks. */
  integertime_t ti_rt_end_min;

  /*! Maximum beginning of (integer) time step in this cell for RT tasks. */
  integertime_t ti_rt_beg_max;

  /*! Minimum (integer) time step size in this cell for RT tasks. */
  integertime_t ti_rt_min_step_size;

  /*! Number of #part updated for RT in this cell */
  int updated;
};
