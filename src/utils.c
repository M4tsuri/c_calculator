#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "utils.h"

void panic(int line, char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "%s[ERROR]%s line %d: ", FAIL, ENDC, line);
    vfprintf(stderr, msg, args);
    va_end(args);
    exit(-1);
}

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
    file = fopen(argv[1], "r");
    if (file == NULL) {
        log_error("specified file not found or corrupted.");
        exit(-1);
    }
    return file;
}

FILE *get_dest(int argc, char **argv) {
    FILE *file = NULL;
    file = fopen(argv[2], "w");
    if (file == NULL) {
        panic(0, "cannot open output file, check your permmision.");
    }
    return file;
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