#include <stdio.h>

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
