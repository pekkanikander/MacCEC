/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol dispatching.
 *
 * This file has been explicitly placed in public domain.
 */

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"

#include <assert.h>

int
p8_dispatch(p8s_frame_t *iframe,
            const proto_dispatch_table_t *dt,
            proto_callback_arg_t *cba_table) {

    assert_frame_invariant(iframe);

    /* NB.  oframe in the stack.  Fix later if needed for the kernel. */
    p8_frame_t oframe = {
        .f_sta = oframe.f_buf,
        .f_end = oframe.f_buf,
    };

    const int rv = p8_decode(iframe, &oframe);
    if (rv < 0) return rv;

    const proto_char_t code = rv;

    assert_frame_invariant(iframe);

    return proto_dispatch(code, &oframe, dt, cba_table);
}
