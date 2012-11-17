/**
 * Pulse Eight serial protocol.
 *
 * Test p8_cec_rx & p8_cec_tx.
 *
 * Copyright (c) 2012 Pekka Nikander.  Placed in public domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <proto.h>

#include <cec.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_dispatch.h>
#include <p8_io.h>

#include <cec_rx.h>
#include <cec_tx.h>

#include <p8_cec_rx.h>
#include <p8_cec_tx.h>

#define DEBUG(...) printf(__VA_ARGS__)


int
rx_callback(proto_char_t code,
            const proto_frame_t *frame,
            proto_callback_arg_t cb_arg) {
    DEBUG("Received: Code 0x%02.2x len %ld char %02.2x\n",
          code, frame->f_end - frame->f_sta, frame->f_sta[0]);
    return 0;
}

int
callback(proto_char_t code,
         const proto_frame_t *frame,
         proto_callback_arg_t cb_arg)  {
    DEBUG("Callback: Code 0x%02.2x len %ld cb_arg 0x%02.2x\n",
          code, frame->f_end - frame->f_sta, cb_arg.cba_int);
    return 0;
}

static const proto_callback_t callbacks[] = {
    callback, rx_callback,
    callback, callback,
    callback, callback,
};

static const proto_dispatch_table_t dt = {
    .dt_number    = COUNT_OF(callbacks),
    /* err, err, rx, ack, nack, tx, err, err, build */
    .dt_indices   = P8_DISPATCH_INDEX_TABLE(0, 0, 1, 2, 3, 4, 0, 0, 5),
    .dt_callbacks = callbacks,
};



int main(void) {
    int fd, rv, len = 0;

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    p8_io_buffer_t pib = {
        .pib_read = 0,
        .pib_last = 0,
    };

    cec_rx_frame_t crb = {
        .f_sta = crb.f_buf,
        .f_end = crb.f_buf,
        .f_status = CEC_RX_EMPTY,
    };

    rv = p8_command_ping(fd, &pib, &crb);
    assert(rv == 0);

    rv = p8_command_set_controlled(fd, &pib, &crb);
    assert(rv == 0);

    /* CEC POLL src:dst = Unknown:TV */
    cec_tx_frame_t ctb = {
        .f_buf = { 0xf0, 0x8c },
        .f_sta = ctb.f_buf,
        .f_end = ctb.f_buf + 2,
        .f_status = CEC_TX_UNKNOWN,
    };
    p8_cec_tx(fd, 3, &ctb, &crb, &pib);

    DEBUG("crb.sta %ld crb.end %ld crb.status %d\n",
          crb.f_sta - crb.f_buf, crb.f_end - crb.f_buf, crb.f_status);

    p8_cec_rx(fd, &crb, &pib);

    len = crb.f_end - crb.f_sta;
    DEBUG("Received %d chars:", len);
    for (int i = 0; i < len; i++)
        DEBUG(" 0x%02.2x", crb.f_sta[i]);
    DEBUG("\n");

    return 0;
}
