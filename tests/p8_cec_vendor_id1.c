/**
 * Pulse Eight serial protocol.
 *
 * Test CEC ping & vendor ID.
 *
 * Copyright (c) 2012 Pekka Nikander.  Placed in public domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

#include <proto.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_dispatch.h>
#include <p8_io.h>

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

static proto_callback_arg_t args[COUNT_OF(callbacks)] = {
    { -1 }, { 0 },
    { P8_IND_ACK }, { P8_IND_NACK },
    { P8_IND_TX_SUCCEEDED },
    { P8_QRY_FIRMWARE },
};

static const proto_dispatch_table_t dt = {
    .dt_number    = COUNT_OF(callbacks),
    /* err, err, rx, ack, nack, tx, err, err, build */
    .dt_indices   = P8_DISPATCH_INDEX_TABLE(0, 0, 1, 2, 3, 4, 0, 5),
    .dt_callbacks = callbacks,
};


int
main(void) {
    int fd, len = 0, rv;
    proto_char_t c;

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    p8s_frame_t sframe = {
        .f_sta = sframe.f_buf,
        .f_end = sframe.f_buf,
    };

    p8_encode_cmd(&sframe, P8_CMD_PING,        NULL, 0); /* -> ACK */
    c = 1;
    p8_encode_cmd(&sframe, P8_SET_CONTROLLED,  &c,   1); /* -> ACK */
    p8_encode_cmd(&sframe, P8_QRY_FIRMWARE,    NULL, 0); /* -> FIRMWARE */
    c = 3;
    p8_encode_cmd(&sframe, P8_CMD_TX_SET_IDLE, &c,   1); /* -> ACK */
    /* CEC POLL src:dst = Unknown:TV */
    c = 0xf0;
    p8_encode_cmd(&sframe, P8_CMD_TX_EOM,      &c,   1); /* -> ACK */
    /* -> TX_SUCCEEDED */
    /* CEC Give Device VENDOR ID: src:dst = Unknown:TV */
    c = 0xf0;
    p8_encode_cmd(&sframe, P8_CMD_TX,          &c,   1); /* -> ACK */
    c = 0x8c;
    p8_encode_cmd(&sframe, P8_CMD_TX_EOM,      &c,   1); /* -> ACK */
    /* -> TX_SUCCEEDED */

    len = sframe.f_end - sframe.f_sta;
    if (len != write(fd, sframe.f_sta, len)) {
        perror("write");
        exit(1);
    }

    fcntl(fd, F_SETFL, O_NONBLOCK);

    sleep(1);             /* NB.  Avoid repetive reads.  Ugly. */

    for (int i = 0; i < 2; i++) {
        sframe.f_end = sframe.f_sta = sframe.f_buf;

        while ((len = read(fd, sframe.f_buf, sizeof(sframe.f_buf))) > 0) {
            sframe.f_end += len;
            DEBUG("Buffer: ");
            for (int i = 0; i < len; i++) {
                DEBUG(" 0x%02.2x", sframe.f_sta[i]);
            }
            DEBUG("\n");
            while (sframe.f_end > sframe.f_sta) {
                rv = p8_dispatch(&sframe, &dt, args);
                if (rv) break;
            }
            sleep(1);             /* NB.  Avoid repetive reads.  Ugly. */
            sframe.f_end = sframe.f_sta = sframe.f_buf;
        }
    }

    return 0;
}
