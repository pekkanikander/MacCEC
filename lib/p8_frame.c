/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol framing.
 *
 * This file has been explicitly placed in public domain.
 */

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"

#include <assert.h>

/**
 * Encodes a P8 frame into the given buffer frbuf.
 *
 * @Returns the number of bytes placed in the frbuf.
 */
int
p8_encode(p8_frame_t *frbuf, p8_code_t code, const p8_param_t *params, p8_len_t param_len) {
    p8_frame_t *p = frbuf;

    assert(code < p8_code_type_count);
    assert(p8_code_types[code].ct_type & P8_CT_H2A);
    assert(param_len >= p8_code_types[code].ct_h2a_min_plen);
    assert(param_len <= p8_code_types[code].ct_h2a_max_plen);

    *p++ = P8_FRAME_BEGIN;
    *p++ = code;
    while (param_len-- > 0) {
        p8_frame_t c = *params++;
        if (c >= P8_FRAME_ESC) {
            *p++ = P8_FRAME_ESC;
            c   -= P8_FRAME_ESC_OFFSET;
        }
        *p++ = c;
    }
    *p++ = P8_FRAME_END;

    return p - frbuf;
}

/**
 * Decodes a P8 frame and call the callback with the frame.
 *
 * Calls p8_frame_error on framing errors.
 * Fails an assert (aborts) on presumed protocol errors.
 * Fix the code tables in p8_code.c if you get an assert failure here.
 *
 * Must be called with a complete frame.  If called with a partial
 * frame, will call p8_frame_error with P8_ERROR_FRAME_END.
 *
 * @Returns the number of bytes consumed, or on error whatever
 * p8_frame_error returns.
 */
int
p8_decode(const p8_frame_t *frbuf, p8_len_t len,
          p8_callback_t callback, p8_callback_arg_t cb_arg) {

    const p8_frame_t *p = frbuf;
    p8_code_t code;
    p8_param_t params[P8_FRAME_BUFFER_LENGTH];

    if (len < 3)
        return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, frbuf, len);

    if (*p++ != P8_FRAME_BEGIN)
        return p8_frame_error(P8_ERROR_FRAME_BEGIN_MISSING, frbuf, len);

    code = *p++;

    p8_param_t *q = params;

    while (*p != P8_FRAME_END && (p - frbuf) < len) {
        if (*p == P8_FRAME_BEGIN)
            return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, frbuf, len);
        if (*p == P8_FRAME_ESC) {
            if (++p - frbuf >= len)
                return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, frbuf, len);
            *q++ = *p++ - P8_FRAME_ESC_OFFSET;
        } else
            *q++ = *p++;
    }

    if (*p++ != P8_FRAME_END)
        return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, frbuf, len);

    assert((code & P8_CODE_MASK) < p8_code_type_count);
    assert(p8_code_types[(code & P8_CODE_MASK)].ct_type & P8_CT_A2H);
    assert(q - params >= p8_code_types[(code & P8_CODE_MASK)].ct_a2h_min_plen);
    assert(q - params <= p8_code_types[(code & P8_CODE_MASK)].ct_a2h_max_plen);

    callback(code, params, q - params, cb_arg);

    return p - frbuf;
}
