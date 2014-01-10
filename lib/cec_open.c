/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"
#include "cec_rx.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_io.h"

int
cec_open(void) {
    int fd, rv;

    fd = p8_open();
    if (fd < 0) return fd;

    PROTO_FRAME(cec_rx_frame_t, iframe, 0, CEC_RX_EMPTY);

    rv = p8_command_ping(fd, iframe);
    assert(rv == 0);

    rv = p8_command_set_controlled(fd, iframe);
    assert(rv == 0);

    return fd;
}
