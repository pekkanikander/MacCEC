/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol command sending.
 *
 * This file has been explicitly placed in public domain.
 */

#include "proto.h"

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

enum status { P8_ERROR=-1, P8_ACK=0, P8_NACK=1, };

static int
p8_command_ack(proto_char_t code,
               const p8_frame_t *params,
               proto_callback_arg_t cba) {
    assert_code_rx_invariant(code, params);
    int *statusp = cba.cba_intp;
    *statusp = P8_ACK;
    DEBUG("ACK received\n");
    return 0;
}

static int
p8_command_nack(proto_char_t code,
                const p8_frame_t *params,
                proto_callback_arg_t cba) {
    assert_code_rx_invariant(code, params);
    int *statusp = cba.cba_intp;
    *statusp = P8_NACK;
    DEBUG("NACK received\n");
    return 0;
}

const proto_callback_t p8_command_callbacks[P8_DISPATCH_DEFAULT_INDEX_TABLE_LENGTH] = {
    P8_DISPATCH_DEFAULT_VECTOR,
    p8_command_ack, p8_command_nack,
    p8_error, p8_error,
    p8_error, p8_error,
};

const static struct proto_dispatch_table p8_command_dt = {
    .dt_indices   = P8_DISPATCH_DEFAULT_INDEX_TABLE,
    .dt_number    = COUNT_OF(p8_command_callbacks),
    .dt_callbacks = p8_command_callbacks,
};

/**
 * Sends the P8 command, and waits for an ACK or NACK.
 * @returns -1 on communication or protocol error.
 * 0 on ACK, 1 on NACK.
 */

int
p8_command(int fd, p8_code_t code,
           proto_char_t *params, proto_len_t len,
           p8_io_buffer_t *pib,
           cec_rx_frame_t *iframe) {
    assert_frame_invariant(iframe);

    p8s_frame_t oframe = {
        .f_sta = oframe.f_buf,
        .f_end = oframe.f_buf,
        .f_status = 0,
    };

    p8_encode_cmd(&oframe, code, params, len);

    int status = P8_ERROR;

    proto_callback_arg_t cba_table[COUNT_OF(p8_command_callbacks)] = {
        { .cba_int = 0 },           /* Protocol error */
        { .cba_frame = iframe },    /* CEC receive error */
        { .cba_frame = iframe },    /* CEC receive callback */
        { .cba_intp = &status },    /* ACK */
        { .cba_intp = &status },    /* NACK */
        { .cba_int = 0 },           /* TX */
        { .cba_int = 0 },           /* TX error/timeout */
        { .cba_int = 0 },           /* FW reply -> protocol error */
        { .cba_int = 0 },           /* BUILDDATE reply -> protocol error */
    };

    if (p8_write(fd, &oframe, pib, &p8_command_dt, cba_table) < 0) {
        status = P8_ERROR;
    }

    assert_frame_invariant(iframe);

    return status;
}

int
p8_command1(int fd, enum p8_code code, proto_char_t param, p8_io_buffer_t *pib, proto_frame_t *iframe) {
    proto_char_t c = param;
    return p8_command(fd, code, &c, 1, pib, iframe);
}
