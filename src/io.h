/**
 * Functions related to I/O.
 */

#pragma once

#include "parameters.h"
#include "packing_data_struct.h"

void io_parse_cmdlineargs(int argc, char* argv[], struct parameters* params);
void io_read_params(struct parameters* params);
void io_read_measurement_file(const char* filename, struct packing_data** packing_sequence, size_t *n_elements, const struct parameters* params);

int io_util_check_file_exists(const char* fname);
int io_util_check_dir_exists(const char* dirname);
int io_util_line_is_empty(const char* line);
int io_util_line_is_comment(const char* line);
void io_util_remove_whitespace(char* line);
void io_util_remove_trailing_comments(char* line);
int io_util_split_name_colon_value_present(const char* line, char* varname,
                                           char* varvalue);
int io_util_line_is_measurement_data(const char* line);
void io_util_parse_measurement_data_line(const char* line, enum task_types* task_type, long long* ci_cellID, long long* cj_cellID, int* ci_count, int *cj_count, float* timing);

