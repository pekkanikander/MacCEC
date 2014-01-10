/**
 * Protocol independent dispatch.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

/**
 * Call a dispatch function based on code.
 *
 * First look up the dispatch function index from
 * the dispatch_table_indices, then call the actual
 * dispatch function, passing the code, frame,
 * and the appropriate argument from cba_table.
 *
 * @returns whatever the dispatch function returns.
 */

int
proto_dispatch(const proto_char_t code,
               const proto_frame_t *frame,
               const proto_dispatch_table_t *dt,
               proto_callback_arg_t *cba_table) {

    assert_frame_invariant(frame);

    /* NB. Protect against errors in the header */
    assert(sizeof(*frame) == PROTO_FRAME_SIZE);

    proto_dispatch_index_t index = dt->dt_indices[code & dt->dt_code_mask];
    if (index >= dt->dt_number) {
        dt->dt_error(code, frame, cba_table[index]);
    }

    /* NB.  Tail recursion. */
    return dt->dt_callbacks[index](code, frame, cba_table[index]);
}
