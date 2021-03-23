#include "res_handle.h"
#include "token_types.h"
#include <stdio.h>
#include "pool.h"

// the maxinum length of a token
#define MAX_TOKEN_SIZE 256

typedef struct Buffer {
    unsigned int len;
    unsigned int cur;
    unsigned int cur_line;
    char *src;
} Buffer;

/**
 * create a Buffer object from a opened file
 * @param src: the file struct
 * @return a buffer instance
 */
Buffer *create_buffer(FILE *src);

/**
 * try to match a string in a buffer using regex, if matched, move the buffer pointer to 
 * next non-empty token (ignore space, tab and newline)
 * @param pattern: the pattern we expect in buffer
 * @param dest: if we match the string in buffer, copy it to dest; if dest is null, then do nothing 
 * @param buf: the source Buffer
 * @return TOKEN_RES::TOKEN_UNMATCH if string not matched, and TOKEN_RES::TOKEN_MATCHED vice versa, others when error occurrs
 */
TOKEN_RES match_pattern(const char *pattern, char *dest, Buffer *buf);

/**
 * try to match a string in a buffer, if matched, move the buffer pointer to 
 * next non-empty token (ignore space, tab and newline)
 * @param str: the string we expect in src
 * @param dest: if we match the string in src, copy it to dest; if dest is null, then do nothing 
 * @param src: the source Buffer
 * @return TOKEN_RES::TOKEN_UNMATCH if string not matched, and TOKEN_RES::TOKEN_MATCHED vice versa, others when error occurrs
 */
TOKEN_RES match_str(char *str, Buffer *src);

/**
 * try to match a number (integet or float) in a buffer, if matched, move the buffer pointer to 
 * next non-empty token (ignore space, tab and newline)
 * @param res: the matched number
 * @param src: the source Buffer
 * @return TOKEN_RES::TOKEN_UNMATCH if string not matched, and TOKEN_RES::TOKEN_MATCHED vice versa, others when error occurrs
 */
TOKEN_RES match_num(long double *res, Buffer *src);

/**
 * try to match a string in a buffer using regex
 * @param pattern: the pattern we expect in buffer
 * @param buf: the source Buffer
 * @return TOKEN_RES::TOKEN_UNMATCH if string not matched, and TOKEN_RES::TOKEN_MATCHED vice versa, others when error occurrs
 */
TOKEN_RES peek_pattern(const char *pattern, Buffer *buf);

/**
 * try to match a string in a buffer.
 * @param str: the string we expect in src
 * @param src: the source Buffer
 * @return TOKEN_RES::TOKEN_UNMATCH if string not matched, and TOKEN_RES::TOKEN_MATCHED vice versa, others when error occurrs
 */
TOKEN_RES peek_str(char *str, Buffer *src);

/**
 * try to match a number (integet or float) in a buffer.
 * @param src: the source Buffer
 * @return TOKEN_RES::TOKEN_UNMATCH if string not matched, and TOKEN_RES::TOKEN_MATCHED vice versa, others when error occurrs
 */
TOKEN_RES peek_num(Buffer *src);

/**
 * split the source string into tokens
 * store the tokens in a pool and return the pool
 * @param src: source code string
 * @param len: length of source code
 * @return a pool of tokens
 */
Pool *tokenize(char *src, size_t len);