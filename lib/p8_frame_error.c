/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol frame error handling.
 *
 * This file has been explicitly placed in public domain.
 */

#include <stdio.h>
#include <assert.h>

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"

/**
 * Called on frame decode error by p8_decode.
 *
 * Feel free to replace with your own, more advanced implementation.
 *
 * @Returns always -1.
 */

int
p8_frame_error(enum p8_frame_error type, const p8_frame_t *frame) {
    assert_frame_invariant(frame);
    fprintf(stderr, "P8 frame error: type=%d, len=%ld:",
            type, proto_frame_len(frame));
    for (proto_char_t *p = frame->f_sta; p < frame->f_end; p++) {
        fprintf(stderr, " 0x%02.2x", *p);
    }
    fprintf(stderr, "\n");
    return -1;
}
