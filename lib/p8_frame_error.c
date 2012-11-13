/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol frame error handling.
 *
 * This file has been explicitly placed in public domain.
 */

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"

#include <stdio.h>

/**
 * Called on frame decode error by p8_decode.
 *
 * Feel free to replace with your own, more advanced implementation.
 *
 * @Returns always -1.
 */

int
p8_frame_error(enum p8_frame_error type, const p8_frame_t *frbuf, p8_len_t len) {
    fprintf(stderr, "P8 frame error: type=%d, len=%d\n", type, len);
    return -1;
}
