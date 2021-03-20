#include "../res_handle/res_handle.h"
// the maxinum length of a token
#define MAX_TOKEN_SIZE 256

typedef struct Buffer {
    unsigned int len;
    unsigned int cur;
    char *src;
} Buffer;

/**
 * try to match a string in a buffer, if matched, return true and move the buffer pointer to 
 * next non-empty token (ignore space, tab and newline)
 * @param str: the string we expect in src
 * @param dest: if we match the string in src, copy it to dest; if dest is null, then do nothing 
 * @param src: the source Buffer
 * @return false if string not matched, and true vice versa
 */
TOKEN_RES match_str(char *str, char *dest, Buffer *src);

/**
 * try to match a number (integet or float) in a buffer, if matched, return true and move the buffer pointer to 
 * next non-empty token (ignore space, tab and newline)
 * @param res: the matched number
 * @param src: the source Buffer
 * @return false if number not matched, and true vice versa
 */
TOKEN_RES match_num(long double *res, Buffer *src);
