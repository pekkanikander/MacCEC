/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol framing.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"

static proto_char_t *
encode_char(proto_char_t *p, proto_char_t c) {
    if (c >= P8_FRAME_ESC) {
        *p++ = P8_FRAME_ESC;
        c   -= P8_FRAME_ESC_OFFSET;
    }
    *p++ = c;
    return p;
}

/**
 * Encodes a Serial P8 frame, with given parameters (may be NULL and 0)
 */
void
p8_encode_cmd(p8s_frame_t *oframe, p8_code_t code, proto_char_t *params, proto_len_t len) {

    assert_frame_invariant(oframe);
    assert(len < COUNT_OF(oframe->f_buf));
    assert_code_tx_invariant(code, len);

    proto_char_t *p = oframe->f_end;

    *p++ = P8_FRAME_BEGIN;
    *p++ = code;
    while (len-- > 0) {
        p = encode_char(p, *params++);
    }
    *p++ = P8_FRAME_END;

    oframe->f_end = p;

    assert(p - oframe->f_buf < COUNT_OF(oframe->f_buf));

    assert_frame_invariant(oframe);
}

/**
 * Encodes a Serial P8 frame, with a single CEC character from iframe.
 */
void
p8_encode_tx(p8s_frame_t *oframe, p8_code_t code, p8_frame_t *iframe) {
    proto_char_t *params = iframe->f_sta;

    assert_frame_invariant(iframe);
    assert_frame_invariant(oframe);
    assert_frame_not_empty(iframe);

    assert_code_tx_invariant(code, 1);

    proto_char_t *p = oframe->f_end;

    *p++ = P8_FRAME_BEGIN;
    *p++ = code;
    p = encode_char(p, *params++);
    *p++ = P8_FRAME_END;

    oframe->f_end = p;
    iframe->f_sta = params;

    assert(p - oframe->f_buf < COUNT_OF(oframe->f_buf));

    assert_frame_invariant(iframe);
    assert_frame_invariant(oframe);
}

