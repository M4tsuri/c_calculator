#include <stdbool.h>

typedef enum _TOKEN_RES {
    TOKEN_MATCHED,
    TOKEN_UNMATCH,
    TOKEN_UNEXPECTED_END,
} TOKEN_RES;

// print an error message and quit
void panic(const char *err_msg);

/**
 * extract successful results from a token matching related result, panic and exit on error
 * @param res: result from a token matching function
 * @param line: line number
 * @return the normal results
 */
TOKEN_RES token_unwrap(TOKEN_RES res, unsigned line);