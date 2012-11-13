/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

#include "cec.h"
#include "cec_tx.h"

#include <stdio.h>

void
cec_tx_error(cec_tx_status_t status, cec_tx_buffer_t *buffer) {
    fprintf(stderr, "CEC TX ERROR: status=%d\n", status);
}

