#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FREE(obj) (obj)->free((obj))

const static char *HEADER = "\033[95m";
const static char *OKBLUE = "\033[94m";
const static char *OKCYAN = "\033[96m";
const static char *OKGREEN = "\033[92m";
const static char *WARNING = "\033[93m";
const static char *FAIL = "\033[91m";
const static char *ENDC = "\033[0m";
const static char *BOLD = "\033[1m";
const static char *UNDERLINE = "\033[4m";

void log_error(const char *msg, ...);
void log_info(const char *msg, ...);
void log_debug(const char *msg, ...);
void log_success(const char *msg, ...);

/**
 * get input file, this file must exist
 */
FILE *get_src(int argc, char **argv);

/**
 * get output file, this file cannot exist before
 */
FILE *get_dest(int argc, char **argv);

// add extra null pointer check
void *s_malloc(size_t size);

// add extra null pointer check
void *s_realloc(void *ptr, size_t size);