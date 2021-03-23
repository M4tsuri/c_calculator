#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "utils.h"

void log_error(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "%s[ERROR]%s ", FAIL, ENDC);
    vfprintf(stderr, msg, args);
    va_end(args);
}

void log_info(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stdout, "%s[INFO]%s ", OKCYAN, ENDC);
    vfprintf(stdout, msg, args);
    va_end(args);
}

void log_debug(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stdout, "%s[DEBUG]%s ", OKBLUE, ENDC);
    vfprintf(stdout, msg, args);
    va_end(args);
}

void log_success(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stdout, "%s[SUCCESS]%s ", OKGREEN, ENDC);
    vfprintf(stdout, msg, args);
    va_end(args);
}

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

// add extra null pointer check on original malloc function
void *s_malloc(size_t size) {
    void *res = malloc(size);
    if (res == NULL) {
        log_error("internal error: error when allocating memory.");
        exit(-1);
    }
    return res;
}

// add extra null pointer check
void *s_realloc(void *ptr, size_t size) {
    void *res = realloc(ptr, size);
    if (res == NULL) {
        log_error("internal error: error when allocating memory.");
        exit(-1);
    }
    return res;
}