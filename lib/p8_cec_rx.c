/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

#include "cec.h"
#include "cec_rx.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"
#include "p8_io.h"

#include "p8_cec_rx.h"

#include <assert.h>

#if P8_RX_FRAME_ACK == (CEC_ACK << 6) && P8_RX_FRAME_EOM == (CEC_EOM << 6)
#define P8_CODE_FLAGS_TO_CEC_FLAGS(f) ((f) >> 6)
#else
#define P8_CODE_FLAGS_TO_CEC_FLAGS(f) \
    ((((f) & P8_RX_FRAME_ACK)? CEC_ACK: 0) | (((f) & P8_RX_FRAME_EOM)? CEC_EOM: 0))
#endif

void
p8_cec_rx_error_cb(p8_code_t code,
                   const p8_param_t *params, p8_len_t param_len,
                   p8_callback_arg_t cba) {
    cec_flags_t flags = P8_CODE_FLAGS_TO_CEC_FLAGS(code);
    cec_rx_buffer_t * const crb = cba.cba_crb;

    cec_rx_error(CEC_RX_ERROR, params[0], flags, crb);
}

/* Called by P8 code on:
    P8_IND_RX_START
    P8_IND_RX_NEXT
    P8_IND_RX_FAILED
*/

void
p8_cec_rx_callback(const p8_code_t code,
                   const p8_param_t *params, p8_len_t param_len,
                   p8_callback_arg_t cba) {

    cec_flags_t flags = P8_CODE_FLAGS_TO_CEC_FLAGS(code);
    cec_rx_buffer_t * const crb = cba.cba_crb;

    const int swcode = (code & P8_CODE_MASK);
    switch (swcode) {
    case P8_IND_RX_START:
        flags |= CEC_START;
        /* Fallthrough */
    case P8_IND_RX_NEXT:
        crb->crb_status = cec_rx_char_cb(params[0], flags, crb);
        break;
    case P8_IND_RX_FAILED:
        crb->crb_status = CEC_RX_ERROR;
        break;
    default:
        DEBUG("Unrecognized RX code 0x%02.2x, swcode = 0x%02.2x\n", code, swcode);
        assert(0);
    }

    if (crb->crb_status != CEC_RX_OK) {
        cec_rx_error(crb->crb_status, params[0], flags, crb);
    }
}

const static p8_callback_t p8_cec_rx_callbacks[] = {
    p8_cec_rx_error_cb,
    p8_cec_rx_callback,
};

const static struct p8_dispatch_table p8_cec_rx_dt;

const static struct p8_dispatch_table p8_cec_rx_dt = {
    .dt_indices   = P8_DISPATCH_INDEX_TABLE(0, 0, 1, 0, 0, 0, 0, 0, 0),
    .dt_number    = COUNT_OF(p8_cec_rx_callbacks),
    .dt_callbacks = p8_cec_rx_callbacks,
};

/**
 * Receives the next CEC message to the given cec_rx_buffer.  Blocks.
 */
extern cec_rx_status_t
p8_cec_rx(int fd, cec_rx_buffer_t *crb, p8_io_buffer_t *pib) {
    assert(crb->crb_char_count == 0);
    assert(crb->crb_status == CEC_RX_OK);

    /* Read until CEC EOM */
    do {
        p8_frame_t frbuf[P8_FRAME_BUFFER_LENGTH];
        int len = 0;

        p8_callback_arg_t cba_table[COUNT_OF(p8_cec_rx_callbacks)] = {
            { .cba_crb = crb }, /* For p8_cec_rx_error_cb */
            { .cba_crb = crb }, /* For p8_cec_rx_callback */
        };

        /* Read at least one P8 frame.  */
        len = p8_read(fd, frbuf, sizeof(frbuf), pib, &p8_cec_rx_dt, cba_table);
        if (len <= 0) {
            return crb->crb_status;
        }
    } while (crb->crb_char_count == 0 || !(crb->crb_flags[crb->crb_char_count - 1] & CEC_EOM));
    return CEC_RX_OK;
}
