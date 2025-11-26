/**
 * Functions related to I/O.
 */

#pragma once

#include "packing_data_struct.h"
#include "parameters.h"
#include "swift_placeholders/timers.h"

#define IO_MAX_LINE_SIZE 256
#define IO_MAX_FILENAME_SIZE 256

void io_parse_cmdlineargs(int argc, char* argv[], struct parameters* params);
void io_read_logged_params(struct parameters* params);
void io_read_logged_events_file(const char* filename,
                                struct packing_data** packing_sequence,
                                int* n_elements,
                                const struct parameters* params);

void io_util_construct_log_filename(char filename[IO_MAX_FILENAME_SIZE],
                                    int threadid, int step,
                                    const struct parameters* params);
int io_util_check_file_exists(const char* fname);
int io_util_check_dir_exists(const char* dirname);
int io_util_line_is_empty(const char* line);
int io_util_line_is_comment(const char* line);
void io_util_remove_whitespace(char* line);
void io_util_remove_trailing_comments(char* line);
int io_util_split_name_colon_value_present(const char* line, char* varname,
                                           char* varvalue);
int io_util_line_is_measurement_data(const char* line);
void io_util_parse_measurement_data_line(const char* line,
                                         struct packing_data* data);
void io_print_timers(const ticks timers_arr[timer_count],
                  const double timings_log_arr[timer_count],
                  const double timings_ratio_min[timer_count],
                  const double timings_ratio_max[timer_count]);

