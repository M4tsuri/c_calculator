#include "log.h"
#include <stdio.h>

void log_error(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "%s[ERROR]%s ", FAIL, ENDC);
    fprintf(stderr, msg, args);
    va_end();
}

void log_info(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stdout, "%s[INFO]%s ", OKCYAN, ENDC);
    fprintf(stdout, msg, args);
    va_end();
}

void log_debug(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stdout, "%s[DEBUG]%s ", OKBLUE, ENDC);
    fprintf(stdout, msg, args);
    va_end();
}

void log_success(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    fprintf(stdout, "%s[SUCCESS]%s ", OKGREEN, ENDC);
    fprintf(stdout, msg, args);
    va_end();
}