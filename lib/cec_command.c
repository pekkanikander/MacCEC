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
#include "cec_device.h"
#include "cec_rx.h"
#include "cec_tx.h"
#include "cec_codes.h"

/**
 * Sends the CEC command.  Waits for a reply, if defined by the state
 * machine.
 *
 * @returns 0 and the reply in the iframe, < 0 on error.
 */

int
cec_command(cec_device_t *d,
            enum cec_code code,
            proto_char_t *prams, proto_len_t len) {
#if 0 /* XXX */
    const cec_tx_flags_t tx_flags = cec_codes_types[cec_code_indices[code]].ct_tx_flags;
    p8_cec_tx(d->d_fd, XXX, tx_flags, &tx_frame, d->d_rx_frame);
#endif
    return 0;
}

