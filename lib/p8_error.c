/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol error reporting.
 *
 * This file has been explicitly placed in public domain.
 */

#include <stdio.h>
#include <assert.h>

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_error.h"

extern int
p8_error(proto_char_t code,
         const p8_frame_t *frame,
         proto_callback_arg_t cb_arg) {
    assert_frame_invariant(frame);
    assert_code_rx_invariant(code, frame);
    fprintf(stderr, "P8 PROTOCOL ERROR: code=0x%02.2x, param_len=%ld",
            code, proto_frame_len(frame));
    for (proto_char_t *p = frame->f_sta; p < frame->f_end; p++)
        fprintf(stderr, ", 0x%02.2x", *p);
    fprintf(stderr, "; cb_arg=0x%p\n", cb_arg.cba_intp);
    return -1;
}

