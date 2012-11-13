/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

#include "cec.h"
#include "cec_rx.h"

cec_rx_status_t
cec_rx_char_cb(cec_char_t c, cec_flags_t f, cec_rx_buffer_t *crb) {

    if (crb->crb_char_count > 0 && (f & CEC_START)) {
        /* Ignore any previous message that didn't include EOM */
        crb->crb_char_count = 0;
    } /* NB.  No "else" here.  A good compiler will optimize. */

    if (crb->crb_char_count > 0 &&
        (crb->crb_flags[crb->crb_char_count] & CEC_EOM)) {
        return CEC_RX_EOM;
    }

    if (crb->crb_char_count >= CEC_MAX_PACKET_LENGTH) {
        crb->crb_status = CEC_RX_OVF;
        return crb->crb_status;
    }

    crb->crb_chars[crb->crb_char_count] = c;
    crb->crb_flags[crb->crb_char_count] = f;
    crb->crb_char_count++;

    return CEC_RX_OK;
}
