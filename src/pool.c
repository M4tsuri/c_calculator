#include "log.h"
#include "pool.h"

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

Pool *create_pool(size_t item_size) {
    Pool *q = s_malloc(sizeof(Pool));
    q->item_num = 0;
    q->item_size = item_size;
    q->max_item_num = 0x100;
    q->buf = s_malloc(q->max_item_num * q->item_size);

    return q;
}

void *use(Pool *p) {
    if (p->item_num == p->max_item_num) {
        p->max_item_num *= 2;
        p->buf = s_realloc(p->buf, p->max_item_num * p->item_size);
    }
    void *res = p->buf + p->item_size * p->item_num;
    p->item_num++;
    return res;
}

void reset_iter(Pool *p) {
    p->cur = p->buf;
}

void *next(Pool *p) {
    if (p->cur == p->buf + p->item_size * p->item_num) {
        return NULL;
    }
    void *res = p->cur;
    p->cur += p->item_size;
    return res;
}

void delete_pool(Pool *p) {
    free(p->buf);
    free(p);
}