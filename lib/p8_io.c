/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol I/O.
 *
 * This file has been explicitly placed in public domain.
 */

#include <unistd.h>
#include <assert.h>

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"
#include "p8_io.h"

int
p8_read_and_dispatch(int fd, p8s_frame_t *iframe,
        const struct proto_dispatch_table *dt, proto_callback_arg_t *cba_table) {

    assert_frame_invariant(iframe);

    int rv;

    /* Read at least one P8 frame.  */
    rv = p8_read(fd, iframe);
    if (rv) return rv;

    while (!proto_frame_is_empty(iframe)) {
        rv = p8_dispatch(iframe, dt, cba_table);
        if (rv) return rv;
    }

    assert_frame_invariant(iframe);

    return 0;
}

int
p8_write(int fd, p8s_frame_t *oframe,
         const struct proto_dispatch_table *dt, proto_callback_arg_t *cba_table) {

    assert_frame_invariant(oframe);

    int slen = proto_frame_len(oframe);
    if (slen != write(fd, oframe->f_sta, slen)) {
        return -1;
    }

    /* NB.  iframe in the stack.  Fix later for kernel if needed. */
    PROTO_FRAME(p8s_frame_t, iframe, 0, 0);
    int rv;
    rv = p8_read_and_dispatch(fd, iframe, dt, cba_table);
    if (rv) return rv;

    assert_frame_invariant(oframe);

    return 0;
}

