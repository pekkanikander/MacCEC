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

void
cec_rx_char_cb(cec_char_t c, cec_flags_t f, cec_rx_frame_t *frame) {
    cec_rx_state_t state = frame->f_status;
    switch (state) {
    case CEC_RX_EMPTY:
        if (!f & CEC_START) {
            cec_rx_error(state, c, f, frame);
            return;
        }
        frame->f_status = CEC_RX_PROGRESS;
        /* Fallthrough */
    case CEC_RX_PROGRESS:
        *frame->f_end++ = c;
        if (f & CEC_EOM)
            frame->f_status = CEC_RX_EOM;
        if (frame->f_end <= frame->f_buf + COUNT_OF(frame->f_buf))
            break;
        /* Fallthrough */
    case CEC_RX_EOM:
        frame->f_status = CEC_RX_OVF;
        /* Fallthrough */
    case CEC_RX_OVF:
    case CEC_RX_ERROR:
    default:
        cec_rx_error(state, c, f, frame);
    }
}
