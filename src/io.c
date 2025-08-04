#include "io.h"

#include "error.h"
#include "help.h"
#include "task.h"

#include <dirent.h>
#include <errno.h>
#include <unistd.h>


/**
 * Read in cmdline args and store relevant runtime parameters
 */
void io_parse_cmdlineargs(int argc, char* argv[], struct parameters* params) {

  int data_dir_given = 0;

  for (int i = 1; i < argc; i++) {
    char* arg = argv[i];
    if (strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0) {
      params->verbose = 1;
    } else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
      params->help = 1;
    } else if (strcmp(arg, "-p") == 0 ||
               strcmp(arg, "--print-each-step") == 0) {
      params->print_each_step = 1;
    } else {
      /* We may have been given the input file.
       * Check whether directory exists. */
      if (io_util_check_dir_exists(arg)) {
        /* that's the one. */
        strcpy(params->data_root_dir, arg);
        data_dir_given = 1;
      } else {
        /* Directory does not exist. No reason to quit,
         * it may be provided as a later arguemnt, he said
         * optimistically. */
        warning("Directory '%s' does not exist", arg);
      }
    }
  }

  if (!params->help && !data_dir_given) {
    warning("No data directory provided, that's a mandatory cmdline arg.");
    help();
  }
}


/**
 * Read in runtime parameters stored in files.
 * In particular, what was stored during the measurement runs.
 */
void io_read_logged_params(struct parameters* params) {

  if (params->verbose) message("Reading run parameters log file.");

  char filepath[IO_MAX_FILENAME_SIZE];
  strcpy(filepath, params->data_root_dir);
  strcat(filepath, "/");
  strcat(filepath, "log_runtime_params.dat");

  if (!io_util_check_file_exists(filepath)) {
    error("Run parameter log file %s doesn't exist", filepath);
  }

  FILE* pfile = fopen(filepath, "r");
  if (pfile == NULL) {
    error("Something went wrong opening the parameter log file.");
  }

  char varname[IO_MAX_LINE_SIZE];
  char varvalue[IO_MAX_LINE_SIZE];
  char tempbuff[IO_MAX_LINE_SIZE];

  while (fgets(tempbuff, IO_MAX_LINE_SIZE, pfile)) {
    if (io_util_line_is_comment(tempbuff)) continue;
    io_util_remove_trailing_comments(tempbuff);
    if (io_util_line_is_empty(tempbuff)) continue;
    if (!io_util_split_name_colon_value_present(tempbuff, varname, varvalue)) {
      warning("Invalid line in parameter log file: '%s'; Skipping it.",
              tempbuff);
    }

#ifdef SWIFT_DEBUG_CHECKS
    message("Got name='%s', value='%s'", varname, varvalue);
#endif

    if (strcmp(varname, "nr_threads") == 0) {
      params->nr_threads = atoi(varvalue);
    } else if (strcmp(varname, "nr_parts") == 0) {
      params->nr_parts = atoll(varvalue);
    } else if (strcmp(varname, "nr_steps") == 0) {
      params->nr_steps = atoi(varvalue);
    } else {
      message("Unrecognised parameter '%s' in file", varname);
    }
  }

  validate_read_params(params);
}


/**
 * Read the entire measurement data from a given file and return the
 * data as an array of pack_data structs to the array `packing_sequence`.
 *
 * `packing_sequence` must point to NULL at entry, and memory will be allocated
 * within this function call.
 * `packing_sequence` will contain `n_elements` entries.
 */
void io_read_logged_events_file(const char* filename,
                                struct packing_data** packing_sequence,
                                size_t* n_elements,
                                const struct parameters* params) {

  if (*packing_sequence != NULL)
    error("packing_sequence array is already allocated.");

  if (!io_util_check_file_exists(filename))
    error("File '%s' not found.", filename);

  FILE* f_p = fopen(filename, "r");
  if (f_p == NULL) error("Error opening '%s'.", filename);

  /* First, let's see how many lines with actual content we have to read */
  int nlines = 0;
  char tempbuff[IO_MAX_LINE_SIZE];

  /* If there is an error with a too long line that doesn't fit into
   * IO_MAX_LINE_SIZE, the parsing of the line later should catch it. */
  while (fgets(tempbuff, IO_MAX_LINE_SIZE, f_p)) {
    if (io_util_line_is_measurement_data(tempbuff)) nlines++;
  }

  /* rewind */
  if (fseek(f_p, 0, SEEK_SET) != 0)
    error("Error rewinding file '%s'", filename);

  /* alloc space for data and store number of elements */
  if (params->verbose) {
    message("Allocating packing sequence containing %d elements, size=%lu",
            nlines, nlines * sizeof(struct packing_data));
  }
  *packing_sequence = malloc(nlines * sizeof(struct packing_data));
  *n_elements = nlines;

  int i = 0;
  while (fgets(tempbuff, IO_MAX_LINE_SIZE, f_p)) {

    /* Are we skipping this particular line? */
    if (!io_util_line_is_measurement_data(tempbuff)) {
#ifdef SWIFT_DEBUG_CHECKS
      io_util_remove_whitespace(tempbuff);
      message(
          "Line doesn't look like measurement line, skipping it.\n\t Line: "
          "'%s'",
          tempbuff);
#endif
      continue;
    }

    enum task_types task_type = task_type_none;
    long long ci_offset = -1;
    long long cj_offset = -1;
    int ci_count = -1;
    int cj_count = -1;
    double timing = -1;

    /* Now get the actual data. */
    io_util_parse_measurement_data_line(tempbuff, &task_type, &ci_offset,
                                        &cj_offset, &ci_count, &cj_count,
                                        &timing);
#ifdef SWIFT_DEBUG_CHECKS
    swift_assert(task_type != task_type_none);
    swift_assert(ci_offset != -1);
    swift_assert(ci_count != -1);
    swift_assert(timing != -1);
    if (task_type == task_type_force_pair ||
        task_type == task_type_gradient_pair ||
        task_type == task_type_density_pair) {
      swift_assert(cj_offset != -1);
      swift_assert(cj_offset != -1);
    }
#endif

    (*packing_sequence)[i].task_type = task_type;
    (*packing_sequence)[i].ci_offset = ci_offset;
    (*packing_sequence)[i].cj_offset = cj_offset;
    (*packing_sequence)[i].ci_count = ci_count;
    (*packing_sequence)[i].cj_count = cj_count;
    (*packing_sequence)[i].timing = timing;

    i++;
  }

  if (i != nlines)
    error("Something went wrong while reading measurement file?");
}


/**
 * Get the data measurement file name to read in
 */
void io_util_construct_log_filename(char filename[IO_MAX_FILENAME_SIZE],
                                    int threadid, int step,
                                    const struct parameters* params) {

  char tempbuff[IO_MAX_FILENAME_SIZE];
  sprintf(tempbuff, "log_thread%03d_step%03d.dat", threadid, step);
  strcpy(filename, params->data_root_dir);
  strcat(filename, "/");
  strcat(filename, tempbuff);

  if (!io_util_check_file_exists(filename)) {
    error("Didn't find measurement log file %s", filename);
  }
}


/**
 * Check whether a file exists. Returns 1 if true.
 */
int io_util_check_file_exists(const char* fname) {
  return access(fname, F_OK) == 0;
}


/**
 * Check whether a directory exists. Returns 1 if true.
 */
int io_util_check_dir_exists(const char* dirname) {
  DIR* dir = opendir(dirname);
  if (dir) {
    /* Directory exists. */
    closedir(dir);
    return 1;
  } else if (ENOENT == errno) {
    /* Directory does not exist. */
    return 0;
  } else {
    /* opendir() failed for some other reason. */
    error("Error in opendir()");
  }
}


/**
 * Check whether this line is empty, i.e. only whitespaces or newlines.
 * returns 1 if true, 0 otherwise. assumes line is IO_MAX_LINE_SIZE
 */
int io_util_line_is_empty(const char* line) {
  int isempty = 0;

  for (int i = 0; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] != ' ') {
      if (line[i] == '\n') {
        isempty = 1;
      }
      break;
    }
  }
  return (isempty);
}


/**
 * Check whether the given line string is a comment, i.e. starts with //
 * or <slash>*
 */
int io_util_line_is_comment(const char* line) {

  /* initialize firsttwo explicily for valgrind */
  char firsttwo[3] = {0, 0, 0};

  strncpy(firsttwo, line, 2);

  /* strcmp returns 0 if strings are equal */
  if (!strcmp(firsttwo, "//") || !strcmp(firsttwo, "/*")) {
    return (1);
  }
  return (0);
}


/**
 * remove heading and trailing whitespaces
 */
void io_util_remove_whitespace(char* line) {

  int start = 0;
  int stop = strlen(line);

  /* find first non-whitespace character */
  for (int i = 0; i < IO_MAX_LINE_SIZE; i++) {
    if ((line[i] != ' ') && (line[i] != '\t')) {
      start = i;
      break;
    }
  }

  /* find last non-whitespace character */
  for (int i = 0; i < stop; i++) {
    if ((line[stop - i - 1] != ' ') && (line[stop - i - 1] != '\t') &&
        (line[stop - i - 1] != '\n')) {
      stop = stop - i - 1;
      break;
    }
  }

  char newline[IO_MAX_LINE_SIZE];
  strncpy(newline, line + start, stop - start + 1);
  newline[stop - start + 1] = '\0';
  strcpy(line, newline);
}


/**
 * Check whether there are trailing comments in this line and if so,
 * remove them.
 */
void io_util_remove_trailing_comments(char* line) {

  for (int i = 0; i < IO_MAX_LINE_SIZE - 2; i++) {
    /* -2: 1 for \0 char, 1 because comment is always 2 characters long,
     * and I only check for the first.*/
    if (line[i] == '\0') {
      break;
    }
    if (line[i] == '/') {
      char twochars[3];
      strncpy(twochars, line + i, 2);
      if (io_util_line_is_comment(twochars)) {
        line[i] = '\n';
        line[i + 1] = '\0';
        break;
      }
    }
  }
}


/**
 * Check that the line you're reading has the correct number of columns,
 * delimited by a column.
 * We expect the line to have the format  `<name>: <value>\n`
 * This function returns 1 if that is the case, 0 if not.
 * If successful, varname will contain <name> and varvalue will contain <value>.
 */
int io_util_split_name_colon_value_present(const char* line, char* varname,
                                           char* varvalue) {

  int pos = 0;
  int check = 0;
  int len = strlen(line);

  /* first check that we have a colon in there */
  for (int i = 0; i < len; i++) {
    if (line[i] == ':') {
      pos = i;
      break;
    }
  }
  if (pos == 0) {
    return 0; /* ':' is either in first place or not present. */
  }

  /* now check that we have non-whitespace characters before the colon */
  for (int i = 0; i < pos; i++) {
    if (line[i] != ' ') {
      check = 1;
      break;
    }
  }

  if (!check) return 0;
  check = 0;

  /* now check that we have non-whitespace characters after the colon */
  for (int i = pos + 1; i < len; i++) {
    if (line[i] != ' ') {
      check = 1;
      break;
    }
  }

  if (!check) return 0;

  strncpy(varname, line, pos);
  varname[pos] = '\0';

  strncpy(varvalue, line + pos + 1, len - pos - 1);
  varvalue[len - pos - 1] = '\0';

  /* message("line: '%s'", line); */
  /* message("line[pos]: '%c'", line[pos]); */
  /* message("name: '%s'", varname); */
  /* message("value: '%s'", varvalue); */
  /* message("pos: %d", pos); */

  io_util_remove_whitespace(varname);
  io_util_remove_trailing_comments(varname);
  io_util_remove_whitespace(varvalue);
  io_util_remove_trailing_comments(varvalue);

  return 1;
}


/**
 * Does the line contain data which look like measurement data?
 * 1 if true.
 */
int io_util_line_is_measurement_data(const char* line) {

  if (io_util_line_is_empty(line)) return 0;
  if (io_util_line_is_comment(line)) return 0;

  const char delim = ',';
  int count = 0;
  int i = 0;
  for (i = 0; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] == '\0' || line[i] == '\n') break;
    if (line[i] == delim) count++;
  }

#ifdef SWIFT_DEBUG_CHECKS
  if (i == IO_MAX_LINE_SIZE) {
    warning("Line trimmed before the end?? '%s'", line);
  }
#endif

  if (count != 5) {
#ifdef SWIFT_DEBUG_CHECKS
    if (count > 0) {
      message("Found line with insufficient number of elements: '%s'", line);
    }
#endif
    return 0;
  }

  return 1;
}


/**
 * Get the measurements out of a line of measurement data
 * into appropriate data types.
 */
void io_util_parse_measurement_data_line(const char* line,
                                         enum task_types* task_type,
                                         long long* ci_offset,
                                         long long* cj_offset, int* ci_count,
                                         int* cj_count, double* timing) {

  char tempbuff[64];
  int prev_delim = 0;

  /* Task type */
  for (int i = 0; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] == '\0' || line[i] == '\n') error("Ended too early?");
    if (line[i] == ',') {
      strncpy(tempbuff, line + prev_delim, i);
      tempbuff[i] = '\0';
      io_util_remove_whitespace(tempbuff);
      prev_delim = i + 1;

      if (strcmp(tempbuff, "density_self") == 0) {
        *task_type = task_type_density_self;
      } else if (strcmp(tempbuff, "density_pair") == 0) {
        *task_type = task_type_density_pair;
      } else if (strcmp(tempbuff, "gradient_self") == 0) {
        *task_type = task_type_gradient_self;
      } else if (strcmp(tempbuff, "gradient_pair") == 0) {
        *task_type = task_type_gradient_pair;
      } else if (strcmp(tempbuff, "force_self") == 0) {
        *task_type = task_type_force_self;
      } else if (strcmp(tempbuff, "force_pair") == 0) {
        *task_type = task_type_force_pair;
      } else {
        error("Unknown read-in task type '%s'", tempbuff);
      }
      break;
    }
  }

  /* Cell i offset */
  for (int i = prev_delim; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] == '\0' || line[i] == '\n') error("Ended too early?");
    if (line[i] == ',') {
      strncpy(tempbuff, line + prev_delim, i - prev_delim);
      tempbuff[i - prev_delim] = '\0';
      io_util_remove_whitespace(tempbuff);
      prev_delim = i + 1;

      *ci_offset = atoll(tempbuff);
      break;
    }
  }

  /* Cell j offset */
  for (int i = prev_delim; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] == '\0' || line[i] == '\n') error("Ended too early?");
    if (line[i] == ',') {
      strncpy(tempbuff, line + prev_delim, i - prev_delim);
      tempbuff[i - prev_delim] = '\0';
      io_util_remove_whitespace(tempbuff);

      *cj_offset = atoll(tempbuff);
      prev_delim = i + 1;
      break;
    }
  }

  /* cell i count */
  for (int i = prev_delim; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] == '\0' || line[i] == '\n') error("Ended too early?");
    if (line[i] == ',') {
      strncpy(tempbuff, line + prev_delim, i - prev_delim);
      tempbuff[i - prev_delim] = '\0';
      io_util_remove_whitespace(tempbuff);

      *ci_count = atoi(tempbuff);
      prev_delim = i + 1;
      break;
    }
  }

  /* cell j count */
  for (int i = prev_delim; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] == '\0' || line[i] == '\n') error("Ended too early?");
    if (line[i] == ',') {
      strncpy(tempbuff, line + prev_delim, i - prev_delim);
      tempbuff[i - prev_delim] = '\0';
      io_util_remove_whitespace(tempbuff);

      *cj_count = atoi(tempbuff);
      prev_delim = i + 1;
      break;
    }
  }

  /* operation timing */
  for (int i = prev_delim; i < IO_MAX_LINE_SIZE; i++) {
    if (line[i] == ',') error("Found too many elements?");
    if (line[i] == '\0' || line[i] == '\n') {
      strncpy(tempbuff, line + prev_delim, i - prev_delim);
      io_util_remove_whitespace(tempbuff);

      *timing = atof(tempbuff);
      prev_delim = i + 1;
      break;
    }
  }
}
