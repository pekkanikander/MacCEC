/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol command sending.
 *
 * This file has been explicitly placed in public domain.
 */

#include "cec.h"
#include "cec_rx.h"

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"
#include "p8_error.h"
#include "p8_io.h"
#include "p8_cec_rx.h"

#include <assert.h>

enum status { P8_ERROR=-1, P8_NACK=0, P8_ACK=1 };

static void
p8_command_ack(p8_code_t code,
               const p8_param_t *params, p8_len_t param_len,
               p8_callback_arg_t cba) {
    int *statusp = cba.cba_intp;
    *statusp = P8_ACK;
}

static void
p8_command_nack(p8_code_t code,
                const p8_param_t *params, p8_len_t param_len,
                p8_callback_arg_t cba) {
    int *statusp = cba.cba_intp;
    *statusp = P8_NACK;
}

const p8_callback_t p8_command_callbacks[P8_DISPATCH_DEFAULT_INDEX_TABLE_LENGTH] = {
    P8_DISPATCH_DEFAULT_VECTOR,
    p8_command_ack, p8_command_nack,
    p8_error, p8_error,
    p8_error, p8_error,
};

const static struct p8_dispatch_table p8_command_dt = {
    .dt_indices   = P8_DISPATCH_DEFAULT_INDEX_TABLE,
    .dt_number    = COUNT_OF(p8_command_callbacks),
    .dt_callbacks = p8_command_callbacks,
};

/**
 * Sends the P8 command, and waits for an ACK or NACK.
 * @returns -1 on communication or protocol error.
 * 1 on ACK, 0 on NACK.
 */

int
p8_command(int fd, p8_code_t code,
           const p8_param_t *params, p8_len_t param_len,
           p8_io_buffer_t *pib,
           cec_rx_buffer_t *crb) {
    p8_frame_t frbuf[P8_FRAME_BUFFER_LENGTH];
    int len, rv;

    len = p8_encode(frbuf, code, params, param_len);

    int status = P8_ERROR;

    p8_callback_arg_t cba_table[COUNT_OF(p8_command_callbacks)] = {
        { .cba_int = 0 },           /* Protocol error */
        { .cba_crb = crb },         /* CEC receive error */
        { .cba_crb = crb },         /* CEC receive callback */
        { .cba_intp = &status },    /* ACK */
        { .cba_intp = &status },    /* NACK */
        { .cba_int = 0 },           /* TX */
        { .cba_int = 0 },           /* TX error/timeout */
        { .cba_int = 0 },           /* FW reply -> protocol error */
        { .cba_int = 0 },           /* BUILDDATE reply -> protocol error */
    };

    if (p8_write(fd, frbuf, len, pib, &p8_command_dt, cba_table) < 0)
        return P8_ERROR;

    return status;
}
