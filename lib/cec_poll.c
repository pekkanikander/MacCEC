/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC pinging.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"
#include "cec_rx.h"
#include "cec_tx.h"
#include "cec_codes.h"

/**
 */
cec_tx_status_t
cec_poll(int fd,
                 cec_header_t addr,
                 cec_rx_frame_t *iframe) {

    assert_frame_invariant(iframe);

    PROTO_FRAME(cec_tx_frame_t, oframe, 1, 0);
    oframe->f_buf[0] = addr;

    int rv = p8_cec_tx(fd, CEC_SIGNAL_FREE_LARGE, 0, oframe, iframe);
    if (rv) return rv;

    return oframe->f_status;
}
