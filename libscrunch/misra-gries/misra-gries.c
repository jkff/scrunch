#include "misra-gries.h"

void mga_init (mga_state *state, int capacity) {
    state -> capacity = capacity;
    state -> storage  = kh_init(mga);
    /*
     * we don't handle the case where we can't allocate the storage, because
     * doing so would be asymmetric to what happens in mga_add.
     */
}

void mga_add (mga_state *state, value_t value) {
    khiter_t iter;
    int ret;
    khash_t(mga) *h = state -> storage;

    iter = kh_get(mga, h, value);


    if (iter != kh_end(h)) {
        kh_value(h, iter)++;
    } else {
        if (kh_size(h) < state -> capacity) {
            /*
             * note that we don't handle the case where put fails to allocate memory!
             * I have no idea how to handle it with the current khash interface.
             */
            iter = kh_put(mga, h, value, &ret);
            kh_value(h, iter) = 1; 
        } else {
            for (iter = kh_begin(h); iter != kh_end(h); ++ iter) {
                if (kh_exist(h, iter)) {
                    if (--kh_value(h, iter) == 0) {
                        kh_del(mga, h, kh_key(h, iter));
                    }
                }
            }
        }
    }
}

void mga_result_init(mga_state *state, mga_result_iterator *iter) {
    iter -> iter = kh_begin(state -> storage);
}

int mga_result_advance(mga_state *state, mga_result_iterator *iter) {
    while (1) {
        iter -> iter++;
        if (iter -> iter == kh_end(state -> storage)) {
            return 0;
        }
        if (kh_exist(state -> storage, iter -> iter)) {
            return 1;
        }
    }
}

value_t mga_result_read(mga_state *state, mga_result_iterator *iter) {
    return kh_key(state -> storage, iter -> iter);
}

void mga_result_end(mga_state *state, mga_result_iterator *iter) {
    /* nothing */
}

void mga_finish(mga_state *state) {
    kh_destroy(mga, state -> storage);
}

