#include "../khash.h"

#define int_hash_func(x) ((uint32_t)x)
#define int_equal(a,b) ((a) == (b))

#define mga_state(name)                     mga_##name##_state
#define mga_result_iterator(name)           mga_##name##_result_iterator
#define mga_init(name, s, capacity)         mga_##name##_init(s, capacity)
#define mga_add(name, s, value)             mga_##name##_add(s, value)
#define mga_result_init(name, s, iter)      mga_##name##_result_init(s, iter)
#define mga_result_advance(name, s, iter)   mga_##name##_result_advance(s, iter)
#define mga_result_read(name, s, iter)      mga_##name##_result_read(s, iter)
#define mga_result_end(name, s, iter)       mga_##name##_result_end(s, iter)
#define mga_end(name, s)                    mga_##name##_end(s)

#define MGA_INIT(name, value_t, value_hash_func, value_equal)                                                   \
    KHASH_INIT(mga_##name, value_t, int, 1, value_hash_func, value_equal)                                       \
                                                                                                                \
    typedef struct {                                                                                            \
        int capacity;                                                                                           \
        khash_t(mga_##name) *storage;                                                                           \
    } mga_##name##_state;                                                                                       \
                                                                                                                \
    typedef struct {                                                                                            \
        khiter_t iter;                                                                                          \
    } mga_##name##_result_iterator;                                                                             \
                                                                                                                \
    void     mga_##name##_init           (mga_##name##_state *state, int capacity);                             \
    void     mga_##name##_add            (mga_##name##_state *state, value_t value);                            \
    void     mga_##name##_result_init    (mga_##name##_state *state, mga_##name##_result_iterator *iter);       \
    int      mga_##name##_result_advance (mga_##name##_state *state, mga_##name##_result_iterator *iter);       \
    value_t  mga_##name##_result_read    (mga_##name##_state *state, mga_##name##_result_iterator *iter);       \
    void     mga_##name##_result_end     (mga_##name##_state *state, mga_##name##_result_iterator *iter);       \
    void     mga_##name##_finish         (mga_##name##_state *state);                                           \
                                                                                                                \
    void mga_##name##_init (mga_##name##_state *state, int capacity) {                                          \
        state -> capacity = capacity;                                                                           \
        state -> storage  = kh_init(mga_##name);                                                                \
        /*                                                                                                      \
         * we don't handle the case where we can't allocate the storage, because                                \
         * doing so would be asymmetric to what happens in mga_add.                                             \
         */                                                                                                     \
    }                                                                                                           \
                                                                                                                \
    void mga_##name##_add (mga_##name##_state *state, value_t value) {                                          \
        khiter_t iter;                                                                                          \
        int ret;                                                                                                \
        khash_t(mga_##name) *h = state -> storage;                                                              \
                                                                                                                \
        iter = kh_get(mga_##name, h, value);                                                                    \
                                                                                                                \
                                                                                                                \
        if (iter != kh_end(h)) {                                                                                \
            kh_value(h, iter)++;                                                                                \
        } else {                                                                                                \
            if (kh_size(h) < state -> capacity) {                                                               \
                /*                                                                                              \
                 * note that we don't handle the case where put fails to allocate memory!                       \
                 * I have no idea how to handle it with the current khash interface.                            \
                 */                                                                                             \
                iter = kh_put(mga_##name, h, value, &ret);                                                      \
                kh_value(h, iter) = 1;                                                                          \
            } else {                                                                                            \
                for (iter = kh_begin(h); iter != kh_end(h); ++ iter) {                                          \
                    if (kh_exist(h, iter)) {                                                                    \
                        if (--kh_value(h, iter) == 0) {                                                         \
                            kh_del(mga_##name, h, kh_key(h, iter));                                             \
                        }                                                                                       \
                    }                                                                                           \
                }                                                                                               \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    void mga_##name##_result_init(mga_##name##_state *state, mga_##name##_result_iterator *iter) {              \
        iter -> iter = kh_begin(state -> storage);                                                              \
    }                                                                                                           \
                                                                                                                \
    int mga_##name##_result_advance(mga_##name##_state *state, mga_##name##_result_iterator *iter) {            \
        while (1) {                                                                                             \
            iter -> iter++;                                                                                     \
            if (iter -> iter == kh_end(state -> storage)) {                                                     \
                return 0;                                                                                       \
            }                                                                                                   \
            if (kh_exist(state -> storage, iter -> iter)) {                                                     \
                return 1;                                                                                       \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    value_t mga_##name##_result_read(mga_##name##_state *state, mga_##name##_result_iterator *iter) {           \
        return kh_key(state -> storage, iter -> iter);                                                          \
    }                                                                                                           \
                                                                                                                \
    void mga_##name##_result_end(mga_##name##_state *state, mga_##name##_result_iterator *iter) {               \
        /* nothing */                                                                                           \
    }                                                                                                           \
                                                                                                                \
    void mga_##name##_end(mga_##name##_state *state) {                                                          \
        kh_destroy(mga_##name, state -> storage);                                                               \
    }                                                                                                           \

