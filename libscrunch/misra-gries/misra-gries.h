#include "khash.h"

typedef int value_t;

#define value_hash_func(x) ((uint32_t)x)
#define value_equal(a,b) ((a) == (b))

KHASH_INIT(mga, value_t, int, 1, value_hash_func, value_equal)

typedef struct {
    int capacity;
    khash_t(mga) *storage;
} mga_state;

typedef struct {
    khiter_t iter;
} mga_result_iterator;

void     mga_init           (mga_state *state, int capacity);
void     mga_add            (mga_state *state, value_t value);
void     mga_result_init    (mga_state *state, mga_result_iterator *iter);
int      mga_result_advance (mga_state *state, mga_result_iterator *iter);
value_t  mga_result_read    (mga_state *state, mga_result_iterator *iter);
void     mga_result_end     (mga_state *state, mga_result_iterator *iter);
void     mga_finish         (mga_state *state);

