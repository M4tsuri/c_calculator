#include "parseopt.h"
#include <sys/stat.h>

FILE *get_src(int argc, char **argv) {
    FILE *file = NULL;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-f") == 0) {
            file = fopen(argv[i + 1], "r");
            if (file == NULL) {
                log_error("specified file not found or corrupted.");
                exit(-1);
            }
            return file;
        }
    }
    log_error("please specify an input file with -f [file].");
    exit(-1);
}

FILE *get_dest(int argc, char **argv) {
    FILE *file = NULL;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0) {
            if (access(argv[i + 1], F_OK) == 0) {
                log_error("the file already exists.");
                exit(-1);
            }
            file = fopen(argv[i + 1], "w");
            return file;
        }
    }
    log_error("please specify an output file with -o [file].");
    exit(-1);
}