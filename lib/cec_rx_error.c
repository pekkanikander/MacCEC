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

#include <stdio.h>

void
cec_rx_error(cec_rx_status_t status, cec_char_t c, cec_flags_t f, cec_rx_buffer_t *buffer) {
    fprintf(stderr, "CEC RX ERROR: status=%d, char=0x%02.2x, flags=0x%02.2x\n",
            status, c, f);
}

