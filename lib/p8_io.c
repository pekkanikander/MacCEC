/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol I/O.
 *
 * This file has been explicitly placed in public domain.
 */

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"
#include "p8_io.h"

#include <unistd.h>
#include <assert.h>

int
p8_read_and_dispatch(int fd, p8s_frame_t *iframe,
        const struct proto_dispatch_table *dt, proto_callback_arg_t *cba_table) {

    assert_frame_invariant(iframe);

    int rv;

    /* Read at least one P8 frame.  */
    rv = p8_read(fd, iframe);
    if (rv) return rv;

    while (iframe->f_end > iframe->f_sta) {
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

    /* NB.  iframe in the stack.  Fix later for kernel if needed. */
    p8s_frame_t iframe = {
        .f_sta = iframe.f_buf,
        .f_end = iframe.f_buf,
    };

    int slen = oframe->f_end - oframe->f_sta;
    if (slen != write(fd, oframe->f_sta, slen)) {
        return -1;
    }

    int rv;
    rv = p8_read_and_dispatch(fd, &iframe, dt, cba_table);
    if (rv) return rv;

    assert_frame_invariant(oframe);

    return 0;
}

