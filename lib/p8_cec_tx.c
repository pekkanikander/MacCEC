/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

#include "cec.h"
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

#include <assert.h>

void
p8_cec_tx_error_cb(p8_code_t code,
                   const p8_param_t *params, p8_len_t params_len,
                   p8_callback_arg_t cba) {
    cec_tx_buffer_t * const ctb = cba.cba_ctb;

    switch (code) {
    case P8_IND_TX_FAIL_LINE:
    case P8_IND_TX_FAIL_ACK:
        ctb->ctb_status = CEC_TX_ERROR;
        cec_tx_error(CEC_TX_ERROR, ctb);
        break;
    case P8_IND_TX_TIMEOUT_DATA:
    case P8_IND_TX_TIMEOUT_LINE:
        ctb->ctb_status = CEC_TX_TIMEOUT;
        cec_tx_error(CEC_TX_TIMEOUT, ctb);
        break;
    default:
        DEBUG("Unrecognized TX failure code 0x%02.2x\n", code);
        assert(0);
    }
}

void
p8_cec_tx_callback(p8_code_t code,
                   const p8_param_t *params, p8_len_t params_len,
                   p8_callback_arg_t cba) {
    cec_tx_buffer_t * const ctb = cba.cba_ctb;

    assert(params_len == 0);
    ctb->ctb_status = CEC_TX_SUCCEEDED;
    DEBUG("CEC TX: SUCCEEDED\n");
}

static void
p8_cec_tx_ack(p8_code_t code,
              const p8_param_t *params, p8_len_t param_len,
              p8_callback_arg_t cba) {
    int *countp = cba.cba_intp;
    (*countp)--;
    DEBUG("CEC TX: ACK received, %d to go\n", *countp);
}

static void
p8_cec_tx_nack(p8_code_t code,
               const p8_param_t *params, p8_len_t param_len,
               p8_callback_arg_t cba) {
    int *countp = cba.cba_intp;
    (*countp)++;
    DEBUG("CEC TX: NACK received: %d", *countp);
}

const p8_callback_t p8_cec_tx_callbacks[P8_DISPATCH_DEFAULT_INDEX_TABLE_LENGTH] = {
    P8_DISPATCH_DEFAULT_VECTOR,
    p8_cec_tx_ack, p8_cec_tx_nack,
    p8_cec_tx_callback, p8_cec_tx_error_cb,
    p8_error, p8_error,
};

const static struct p8_dispatch_table p8_cec_tx_dt = {
    .dt_indices   = P8_DISPATCH_DEFAULT_INDEX_TABLE,
    .dt_number    = COUNT_OF(p8_cec_tx_callbacks),
    .dt_callbacks = p8_cec_tx_callbacks,
};


/*
 * Sends an CEC message through the P8 interface.
 * Blocks until the acks and SUCCEED/FAIL/TIMEOUT has been received.
 */

cec_tx_status_t
p8_cec_tx(int fd, unsigned char idletime,
          cec_tx_buffer_t *ctb, cec_rx_buffer_t *crb, p8_io_buffer_t *pib) {
    int ack_count = 0, nack_count = 0;
    p8_frame_t frbuf[P8_FRAME_BUFFER_LENGTH];
    int len , rv;

    assert(ctb->ctb_char_count > 0);

    len = p8_encode(frbuf, P8_CMD_TX_SET_IDLETIME, &idletime, 1);
    ack_count++;
    for (int i = 0; i < ctb->ctb_char_count - 1; i++) {
        len += p8_encode(frbuf + len, P8_CMD_TX, ctb->ctb_chars + i, 1);
        ack_count++;
        if (len >= sizeof(frbuf)) {
            cec_tx_error(CEC_TX_ERROR, ctb);
            return CEC_TX_ERROR;
        }
    }
    len += p8_encode(frbuf + len, P8_CMD_TX_EOM, ctb->ctb_chars + ctb->ctb_char_count - 1, 1);
    ack_count++;

    ctb->ctb_status = CEC_TX_UNKNOWN;

    p8_callback_arg_t cba_table[COUNT_OF(p8_cec_tx_callbacks)] = {
        { .cba_int = 0 },           /* Protocol error */
        { .cba_crb = crb },         /* CEC receive error */
        { .cba_crb = crb },         /* CEC receive callback */
        { .cba_intp = &ack_count },
        { .cba_intp = &nack_count },
        { .cba_ctb = ctb },         /* TX */
        { .cba_ctb = ctb },         /* TX error/timeout */
        { .cba_int = 0 },           /* FW reply -> protocol error */
        { .cba_int = 0 },           /* BUILDDATE reply -> protocol error */
    };

    DEBUG("CEC TX: Writing %d bytes, waiting for %d acks\n", len, ack_count);
    if (p8_write(fd, frbuf, len, pib, &p8_cec_tx_dt, cba_table) < 0) {
        cec_tx_error(CEC_TX_ERROR, ctb);
        return CEC_TX_ERROR;
    }
    /* Process input until command NACK or TX SUCCESS/FAIL/TIMEOUT received. */
    /* XXX Should have a timeout */
    while (nack_count == 0 && ctb->ctb_status == CEC_TX_UNKNOWN) {
        DEBUG("CEC TX: Processing input\n");
        p8_read(fd, frbuf, sizeof(frbuf), pib, &p8_cec_tx_dt, cba_table);
    }

    if (ack_count != 0 || nack_count != 0) {
        ctb->ctb_status = CEC_TX_ERROR;
    }

    if (ctb->ctb_status != CEC_TX_SUCCEEDED) {
        cec_tx_error(ctb->ctb_status, ctb);
    }

    return ctb->ctb_status;
}

