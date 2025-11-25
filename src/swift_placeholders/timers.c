/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2012 Pedro Gonnet (pedro.gonnet@durham.ac.uk)
 *                    Matthieu Schaller (schaller@strw.leidenuniv.nl)
 *               2016 John A. Regan (john.a.regan@durham.ac.uk)
 *                    Tom Theuns (tom.theuns@durham.ac.uk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

/* Config parameters. */

/* This object's header. */
#include "timers.h"

/* Some standard headers. */
#include <stdio.h>
#include <string.h>

/* Local includes. */
#include "clocks.h"
#include "error.h"
#include "task.h"

/* The timers. */
ticks timers[timer_count];

/* Timer names. */
const char* timers_names[timer_count] = {
    "none",          "density_self", "density_pair", "gradient_self",
    "gradient_pair", "force_self",   "force_pair",
};

/* File to store the timers */
static FILE* timers_file;

/**
 * @brief Re-set all the timers.
 *
 */
void timers_reset_all(void) {

  for (int k = 0; k < timer_count; k++) timers[k] = 0;
}

void timers_reset(ticks* timer) {
  for (int k = 0; k < timer_count; k++) timer[k] = 0;
}

void timing_reset_time(double* time) {
  for (int k = 0; k < timer_count; k++) time[k] = 0.;
}

/**
 * @brief Outputs all the timers to the timers dump file.
 *
 * @param step The current step.
 */
void timers_print_file(int step) {
  fprintf(timers_file, "%d\t", step);
  for (int k = 0; k < timer_count; k++)
    fprintf(timers_file, "%25.3f ", clocks_from_ticks(timers[k]));
  fprintf(timers_file, "\n");
  fflush(timers_file);
}

/**
 * @brief Opens the file to contain the timers info and print a header
 *
 * @param rank The MPI rank of the file.
 */
void timers_open_file(int rank) {

  char buff[100];
  sprintf(buff, "timers_%d.txt", rank);
  timers_file = fopen(buff, "w");
  if (timers_file == NULL) error("Could not create file '%s'.", buff);

  fprintf(timers_file, "# timers: \n# step |");
  for (int k = 0; k < timer_count; k++)
    fprintf(timers_file, "%25s ", timers_names[k]);
  fprintf(timers_file, "\n");
}

/**
 * @brief Close the file containing the timer info.
 */
void timers_close_file(void) { fclose(timers_file); }


/**
 * @brief Print a single line of the table for print_timers()
 */
void print_single_timer_line(
    enum task_types type, enum task_subtypes subtype, enum timer_enum timer,
    const ticks timers_arr[timer_count], const double timers_log_arr[timer_count],
                  const double timings_ratio_min[timer_count],
                  const double timings_ratio_max[timer_count]) {

  double dt_sim = clocks_from_ticks(timers_arr[timer]);
  /* Note that logged time is in microseconds, not milliseconds */
  double dt_log = timers_log_arr[timer] * 1e-3;
  double rat_min = timings_ratio_min[timer];
  double rat_max = timings_ratio_max[timer];

  char taskname[80] = "";
  strcat(taskname, subtaskID_names[subtype]);
  strcat(taskname, "/");
  strcat(taskname, taskID_names[type]);

  printf("| %-20s | %17.5g | %17.5g |  %8.4f |  %8.4f |  %8.4f |\n",
      taskname, dt_sim, dt_log, dt_sim / dt_log, rat_min, rat_max);
}

/**
 * Print output to screen in a nice table.
 */
void print_timers(const ticks timers_arr[timer_count],
                  const double timings_log_arr[timer_count],
                  const double timings_ratio_min[timer_count],
                  const double timings_ratio_max[timer_count]) {

  printf(
      "--------------------------------------"
      "--------------------------------------"
      "------------------------"
      "\n");
  printf(
      "| Name                 | Simulation [ms]   |"
      " Measured log [ms] | Ratio     |"
      " Ratio min | Ratio max |"
      "\n");
  printf(
      "--------------------------------------"
      "--------------------------------------"
      "------------------------"
      "\n");

  print_single_timer_line(task_type_pack, task_subtype_density, timer_density_pack, timers_arr, timings_log_arr, timings_ratio_min, timings_ratio_max);
  print_single_timer_line(task_type_unpack, task_subtype_density, timer_density_unpack, timers_arr, timings_log_arr, timings_ratio_min, timings_ratio_max);
  print_single_timer_line(task_type_pack, task_subtype_gradient, timer_gradient_pack, timers_arr, timings_log_arr, timings_ratio_min, timings_ratio_max);
  print_single_timer_line(task_type_unpack, task_subtype_gradient, timer_gradient_unpack, timers_arr, timings_log_arr, timings_ratio_min, timings_ratio_max);
  print_single_timer_line(task_type_pack, task_subtype_force, timer_force_pack, timers_arr, timings_log_arr, timings_ratio_min, timings_ratio_max);
  print_single_timer_line(task_type_unpack, task_subtype_force, timer_force_unpack, timers_arr, timings_log_arr, timings_ratio_min, timings_ratio_max);

  printf(
      "--------------------------------------"
      "--------------------------------------"
      "------------------------"
      "\n");
}
