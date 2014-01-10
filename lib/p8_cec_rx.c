/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"
#include "cec_device.h"
#include "cec_rx.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"
#include "p8_io.h"
#include "p8_cec_rx.h"

#if P8_RX_FRAME_ACK == (CEC_ACK << 6) && P8_RX_FRAME_EOM == (CEC_EOM << 6)
#define P8_CODE_FLAGS_TO_CEC_FLAGS(f) ((f) >> 6)
#else
#define P8_CODE_FLAGS_TO_CEC_FLAGS(f) \
    ((((f) & P8_RX_FRAME_ACK)? CEC_ACK: 0) | (((f) & P8_RX_FRAME_EOM)? CEC_EOM: 0))
#endif

static const char *CEC_FLAGS_TO_STRING[] = {
    "",
    "ack",
    "eom",
    "ack eom",
    "start",
    "ack start",
    "eom start",
    "ack eom start",
};

int
p8_cec_rx_error_cb(proto_char_t code,
                   const p8_frame_t *frame,
                   proto_callback_arg_t cba) {
    assert_frame_invariant(frame);

    cec_flags_t flags = P8_CODE_FLAGS_TO_CEC_FLAGS(code);
    cec_rx_frame_t * const iframe = cba.cba_frame;

    DEBUG("CEC RX: error callback 0x%02.2x %s\n", code, CEC_FLAGS_TO_STRING[flags]);
    return cec_rx_error(CEC_RX_ERROR, frame->f_sta[0], flags, iframe);
}

/* Called by P8 code on:
    P8_IND_RX_START
    P8_IND_RX_NEXT
    P8_IND_RX_FAILED
*/

int
p8_cec_rx_callback(proto_char_t code,
                   const p8_frame_t *frame,
                   proto_callback_arg_t cba) {
    assert_frame_invariant(frame);

    cec_flags_t flags = P8_CODE_FLAGS_TO_CEC_FLAGS(code);
    cec_rx_frame_t * const iframe = cba.cba_frame;

    const int swcode = (code & P8_CODE_MASK);
    switch (swcode) {
    case P8_IND_RX_START:
        flags |= CEC_START;
    case P8_IND_RX_NEXT:
        DEBUG("CEC RX: char 0x%02.2x %s\n", frame->f_sta[0], CEC_FLAGS_TO_STRING[flags]);
        cec_rx_char_cb(frame->f_sta[0], flags, iframe);
        break;
    default:
        DEBUG("CEC RX: error code 0x%02.2x %s, swcode = 0x%02.2x\n",
              code, CEC_FLAGS_TO_STRING[flags], swcode);
        assert(0);
    }

    assert_frame_invariant(frame);

    return 0;
}

const static proto_callback_t p8_cec_rx_callbacks[] = {
    p8_cec_rx_error_cb,
    p8_cec_rx_callback,
};

const static struct proto_dispatch_table p8_cec_rx_dt = {
    .dt_number    = COUNT_OF(p8_cec_rx_callbacks),
    .dt_code_mask = P8_DISPATCH_CODE_MASK,
    .dt_callbacks = p8_cec_rx_callbacks,
    .dt_indices   = P8_DISPATCH_INDEX_TABLE(0, 1, 0, 0, 0, 0, 0),
};

/**
 * Receives the next CEC message to the given cec_rx_frame.  Blocks.
 */
extern int
p8_cec_rx(int fd, cec_rx_frame_t *iframe) {
    assert_frame_invariant(iframe);
    assert_frame_is_empty(iframe);
    assert(iframe->f_status == CEC_RX_EMPTY);

    /* Read until CEC EOM */
    do {
        /* NB.  The frame is allocated in the stack. */
        PROTO_FRAME(p8_frame_t, p8frame, 0, 0);
        int rv;

        proto_callback_arg_t cba_table[COUNT_OF(p8_cec_rx_callbacks)] = {
            { .cba_frame = iframe }, /* For p8_cec_rx_error_cb */
            { .cba_frame = iframe }, /* For p8_cec_rx_callback */
        };

        DEBUG("CEC RX: Processing input\n");
        rv = p8_read_and_dispatch(fd, p8frame, &p8_cec_rx_dt, cba_table);
        if (rv) return rv;

    } while (proto_frame_is_empty(iframe) || iframe->f_status == CEC_RX_PROGRESS);

    assert_frame_invariant(iframe);

    return 0;
}
