/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC local active source management.
 *
 * This file has been explicitly placed in public domain.
 */

#include "proto.h"

#include "cec.h"
#include "cec_rx.h"
#include "cec_tx.h"

#include <assert.h>

int
cec_out_touch_play(cec_device_t *d) {
    cec_cmd_image_view_on(d);
    cec_cmd_active_source(d);
}
