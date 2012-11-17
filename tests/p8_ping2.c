/**
 * Second ping, testing encode and decode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <proto.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_dispatch.h>
#include <p8_io.h>

static int acked = 0;

int callback(proto_char_t code,
             const proto_frame_t *frame,
             proto_callback_arg_t dummy) {
    assert(code == P8_IND_ACK);
    acked = 1;
    return 0;
}

proto_callback_t callbacks[] = { callback };

const proto_dispatch_table_t dt = {
    .dt_number = 1,
    .dt_indices = { 0, },       /* All zeros */
    .dt_error = callback,
    .dt_callbacks = callbacks,
};

int main(void) {
    int fd, len, rv;

    p8s_frame_t sframe = {
        .f_sta = sframe.f_buf,
        .f_end = sframe.f_buf,
    };

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    p8_encode_cmd(&sframe, P8_CMD_PING, NULL, 0);
    len = write(fd, sframe.f_buf, sframe.f_end - sframe.f_sta);
    if (len != sframe.f_end - sframe.f_sta) {
        perror("write");
        exit(1);
    }

    sleep(1);                  /* NB.  Avoid repetive reads.  Ugly. */

    sframe.f_end = sframe.f_sta = sframe.f_buf;
    len = read(fd, sframe.f_buf, sizeof(sframe.f_buf));
    sframe.f_end += len;

    proto_callback_arg_t cba_table[] = { { .cba_int = 0 } };

    while (sframe.f_end > sframe.f_sta) {
        rv = p8_dispatch(&sframe, &dt, cba_table);
        if (rv) break;
    }

    if (!acked) {
        fprintf(stderr, "No ack received!\n");
        exit (1);
    }

    return rv;
}
