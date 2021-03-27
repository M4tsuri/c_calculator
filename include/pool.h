#include <stdlib.h>

/**
 * a for each macro on a pool
 * for example:
 * Token *item;
 * FOR_EACH(item, token_pool) {
 *     do_something
 * }
 */
#define FOR_EACH(item, pool) for (item = pool_next(pool); item != NULL; item = pool_next(pool))

/**
 *  note: once the Pool is initialized with several pushes, there won't be any push after
 * the first pop.
 * so we do not need to consider , an array is enough.
 */
typedef struct Pool {
    size_t item_size;
    size_t item_num;
    size_t max_item_num;
    void *buf;
    // pointer pointing to current object in pool
    void *cur;
    void (*free)(struct Pool *);
} Pool;

/**
 * create an item pool with specified item size
 * @return a Pool instance
 */
Pool *create_pool(size_t item_size);

/**
 * get a free space in the pool and return a pointer pointing to it
 * we suppose that after calling this method, the returned space will be used
 * so we add p->item_num by one
 */
void *pool_use(Pool *p);

/**
 * set cur pointer to the beginning of buffer
 */
void reset_iter(Pool *p);

/**
 * iterate items in a pool, we will return a pointer to current object on each call
 * and update the p->cur field.
 * remeber to call reset_iter before a second iteration
 */
void *pool_next(Pool *p);

void *pool_peek(Pool *p);

void *pool_peek2(Pool *p);

// free all memory used by the pool
void delete_pool(Pool *p);

void *pool_get(Pool *p, size_t idx);

size_t pool_idx(Pool *p, void *ptr);