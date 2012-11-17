/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"

#include "p8_error.h"
#include "p8_dispatch.h"
#include "p8_io.h"

#include "cec.h"
#include "cec_rx.h"
#include "cec_tx.h"

#include <stdio.h>

void
cec_tx_error(cec_tx_status_t status, cec_tx_frame_t *frame) {
    fprintf(stderr, "CEC TX ERROR: status=%d\n", status);
}

