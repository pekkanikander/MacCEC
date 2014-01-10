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
#include "cec_tx.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_error.h"
#include "p8_dispatch.h"
#include "p8_io.h"

#include "p8_cec_rx.h"
#include "p8_cec_tx.h"

int
p8_cec_tx_error_cb(proto_char_t code,
                   const p8_frame_t *frame,
                   proto_callback_arg_t cba) {
    cec_tx_frame_t * const oframe = cba.cba_frame;

    assert_frame_invariant(frame);
    assert_code_rx_invariant(code, frame);

    DEBUG("CEC_TX: ERROR: p8_tx_error: code=0x%02.2x\n", code);

    switch (code) {
    case P8_IND_TX_FAIL_ACK:
        /* Not acked.  Part of the protocol. */
        oframe->f_status = CEC_TX_NO_ACK;
        break;
    case P8_IND_TX_FAIL_LINE:
        cec_tx_error(CEC_TX_ERROR, oframe);
        break;
    case P8_IND_TX_TIMEOUT_D:
    case P8_IND_TX_TIMEOUT_L:
        cec_tx_error(CEC_TX_TIMEOUT, oframe);
        break;
    default:
        DEBUG("Unrecognized TX failure code 0x%02.2x\n", code);
        assert(0);
    }
    return -1;
}

int
p8_cec_tx_callback(proto_char_t code,
                   const p8_frame_t *frame,
                   proto_callback_arg_t cba) {
    cec_tx_frame_t * const oframe = cba.cba_frame;

    assert_code_rx_invariant(code, frame);
    assert_frame_invariant(frame);
    assert_frame_is_empty(frame);

    oframe->f_status = CEC_TX_SUCCEEDED;
    DEBUG("CEC TX: SUCCEEDED\n");
    return 0;
}

static int
p8_cec_tx_ack(proto_char_t code,
              const p8_frame_t *frame,
              proto_callback_arg_t cba) {
    assert_code_rx_invariant(code, frame);
    assert_frame_invariant(frame);
    assert_frame_is_empty(frame);

    int *countp = cba.cba_intp;
    (*countp)--;
    DEBUG("CEC TX: ACK received, %d to go\n", *countp);
    return 0;
}

static int
p8_cec_tx_nack(proto_char_t code,
               const p8_frame_t *frame,
               proto_callback_arg_t cba) {
    assert_code_rx_invariant(code, frame);
    assert_frame_invariant(frame);
    assert_frame_is_empty(frame);

    int *countp = cba.cba_intp;
    (*countp)++;
    DEBUG("CEC TX: NACK received: %d", *countp);
    return 0;
}

const proto_callback_t p8_cec_tx_callbacks[P8_DISPATCH_DEFAULT_INDEX_TABLE_LENGTH] = {
    P8_DISPATCH_DEFAULT_VECTOR,
    p8_cec_tx_ack, p8_cec_tx_nack,
    p8_cec_tx_callback, p8_cec_tx_error_cb,
    p8_error,
};

const static struct proto_dispatch_table p8_cec_tx_dt = {
    .dt_number    = COUNT_OF(p8_cec_tx_callbacks),
    .dt_code_mask = P8_DISPATCH_CODE_MASK,
    .dt_callbacks = p8_cec_tx_callbacks,
    .dt_indices   = P8_DISPATCH_DEFAULT_INDEX_TABLE,
};


/*
 * Sends an CEC message through the P8 interface.
 * Blocks until the acks and SUCCEED/FAIL/TIMEOUT has been received.
 */

int
p8_cec_tx(int fd, cec_signal_free_time_t idletime, cec_tx_flags_t flags,
          cec_tx_frame_t *cec_oframe, cec_rx_frame_t *cec_iframe) {
    int ack_count = 0, nack_count = 0;

    assert_frame_invariant(cec_oframe);
    assert_frame_invariant(cec_iframe);

    PROTO_FRAME(p8_frame_t, p8frame, 0, 0);

    proto_callback_arg_t cba_table[COUNT_OF(p8_cec_tx_callbacks)] = {
        { .cba_int = 0 },            /* Protocol error */
        { .cba_frame = cec_iframe }, /* CEC receive error */
        { .cba_frame = cec_iframe }, /* CEC receive callback */
        { .cba_intp = &ack_count },
        { .cba_intp = &nack_count },
        { .cba_frame = cec_oframe }, /* TX */
        { .cba_frame = cec_oframe }, /* TX error/timeout */
        { .cba_int = 0 },            /* FW reply -> protocol error */
    };

    /* Construct a p8 frame and write it out */
    {
        proto_char_t param;

        param = idletime;
        p8_encode_cmd(p8frame, P8_CMD_TX_SET_IDLE, &param, 1);
        ack_count++;

        if (flags & CEC_TX_BROADCAST) {
            param = 1;
            p8_encode_cmd(p8frame, P8_CMD_TX_SET_ACK_P, &param, 1);
            ack_count++;
        }

        while (cec_oframe->f_sta < cec_oframe->f_end - 1) {
            p8_encode_tx(p8frame, P8_CMD_TX, cec_oframe);
            ack_count++;
        }

        p8_encode_tx(p8frame, P8_CMD_TX_EOM, cec_oframe);
        ack_count++;

        cec_oframe->f_status = CEC_TX_UNKNOWN;

        DEBUG("CEC TX: Writing %ld bytes, waiting for %d acks\n",
              proto_frame_len(p8frame), ack_count);
        int rv = p8_write(fd, p8frame, &p8_cec_tx_dt, cba_table);
        if (rv) {
            cec_tx_error(CEC_TX_ERROR, cec_oframe);
            return rv;
        }
    }

    /* Process input until command NACK or TX SUCCESS/FAIL/TIMEOUT received. */
    {
        proto_frame_init(p8frame, 0);
        /* XXX Should have a timeout */
        while (nack_count == 0 && cec_oframe->f_status == CEC_TX_UNKNOWN) {
            DEBUG("CEC TX: Processing input\n");
            p8_read_and_dispatch(fd, p8frame, &p8_cec_tx_dt, cba_table);
        }
    }

    if (ack_count != 0 || nack_count != 0) {
        cec_tx_error(CEC_TX_ERROR, cec_oframe);
    }

    assert_frame_invariant(cec_oframe);
    assert_frame_invariant(cec_iframe);

    return 0;
}

