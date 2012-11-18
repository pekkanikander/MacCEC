/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

#include "proto.h"

#include "cec.h"
#include "cec_rx.h"
#include "cec_tx.h"
#include "cec_codes.h"

#include <assert.h>

/**
 * Sends the CEC command.  Waits for a reply, if defined by the state
 * machine.
 *
 * @returns 0 and the reply in the iframe, < 0 on error.
 */

int
cec_command(int fd,
            cec_header_t addr,
            enum cec_code code,
            proto_char_t *prams, proto_len_t len,
            cec_rx_frame_t *iframe) {

    assert_frame_invariant(iframe);

    return 0;
}

/**
 * @returns 0 if succeeded.
 * < 0 if sending failed.
 * > 0 if there was a timeout.
 */
int
cec_command_poll(int fd,
                 cec_header_t addr,
                 cec_rx_frame_t *iframe) {

    assert_frame_invariant(iframe);

    cec_tx_frame_t oframe = {
        .f_buf = { addr },
        .f_sta = oframe.f_buf,
        .f_end = oframe.f_buf + 1,
    };

    int rv = p8_cec_tx(fd, CEC_SIGNAL_FREE_LARGE, &oframe, iframe);
    if (rv) return rv;

    return oframe.f_status;
}
