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

#include <stdio.h>

int
cec_rx_error(cec_rx_frame_state_t state, cec_char_t c, cec_flags_t f, cec_rx_frame_t *frame) {
    fprintf(stderr, "CEC RX ERROR: state=%d, char=0x%02.2x, flags=0x%02.2x\n",
            state, c, f);
    frame->f_status = CEC_RX_ERROR;
    return -1;
}

