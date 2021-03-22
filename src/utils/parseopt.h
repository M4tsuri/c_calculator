#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"

/**
 * get input file, this file must exist
 */
FILE *get_src(int argc, char **argv);

/**
 * get output file, this file cannot exist before
 */
FILE *get_dest(int argc, char **argv);