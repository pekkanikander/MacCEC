/**
 * P8 Serial protocol.
 *
 * Serial protocol framing.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"

/**
 * Decodes a Serial P8 frame.
 *
 * Calls p8_frame_error on framing errors.
 *
 * Must be called with a complete frame.  If called with a partial
 * frame, will call p8_frame_error with P8_ERROR_FRAME_END.
 *
 * @returns The P8 code word, or a value less than zero on error.
 */
int
p8_decode(p8s_frame_t *iframe, p8_frame_t *oframe) {

    assert_frame_invariant(iframe);
    assert_frame_invariant(oframe);

    proto_char_t *end = iframe->f_end;
    proto_char_t *p   = iframe->f_sta;
    proto_len_t   len = end - p;
    proto_char_t *q   = oframe->f_end;

    if (len < 3)
        return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, iframe);

    if (*p++ != P8_FRAME_BEGIN)
        return p8_frame_error(P8_ERROR_FRAME_BEGIN_MISSING, iframe);

    const proto_char_t code = *p++;
    const proto_char_t p8_code = code & P8_CODE_MASK;

    while (*p != P8_FRAME_END && p < end) {
        if (*p == P8_FRAME_BEGIN)
            return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, iframe);
        if (*p == P8_FRAME_ESC) {
            if (++p > end)
                return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, iframe);
            *q++ = *p++ - P8_FRAME_ESC_OFFSET;
        } else
            *q++ = *p++;
    }

    if (*p++ != P8_FRAME_END)
        return p8_frame_error(P8_ERROR_FRAME_INCOMPLETE, iframe);

    if (   (p8_code >= P8_CODE_TYPE_COUNT)
        || (p8_code_types[p8_code].ct_type & P8_CT_A2H) == 0
        || (q - oframe->f_end < p8_code_types[p8_code].ct_a2h_min_plen)
        || (q - oframe->f_end > p8_code_types[p8_code].ct_a2h_max_plen)) {
        return p8_frame_error(P8_ERROR_FRAME_MALFORMED, iframe);
    }

    iframe->f_sta = p;
    oframe->f_end = q;

    assert_frame_invariant(iframe);
    assert_frame_invariant(oframe);

    return code;
}
