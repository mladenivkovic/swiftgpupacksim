/**
 * Functions related to I/O.
 */

#pragma once

#include "parameters.h"

void io_parse_cmdlineargs(int argc, char *argv[], struct parameters *params);
void io_read_params(struct parameters *params);

int io_util_check_file_exists(const char* fname);
int io_util_check_dir_exists(const char* dirname);
int io_util_line_is_empty(const char* line);
int io_util_line_is_comment(const char* line);
void io_util_remove_whitespace(char* line);
void io_util_remove_trailing_comments(char* line);
int io_util_split_name_colon_value_present(const char* line, char* valname, char* varvalue);
