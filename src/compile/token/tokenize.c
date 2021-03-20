#include "tokenize.h"
#include "../res_handle/res_handle.h"
#include <stdlib.h>
#include <string.h>

TOKEN_RES match_str(char *str, char *dest, Buffer *src) {
    unsigned str_len = strlen(str);
    if (str_len > src->len - src->cur)
    if (strcmp(str, &src->src[src->cur]) == 0) {

    }
}

TOKEN_RES match_num(long double *res, Buffer *src) {
    
}