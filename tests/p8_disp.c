/**
 * Test dispatch.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_dispatch.h>
#include <p8_io.h>

#define DEBUG(...) printf(__VA_ARGS__)


void callback(p8_code_t code, const
              p8_param_t *params, p8_len_t param_len,
              p8_callback_arg_t cb_arg) {
    DEBUG("Callback: Code 0x%02.2x len %d cb_arg 0x%02.2x\n", code, param_len, cb_arg.cba_int);
}

static const p8_callback_t callbacks[] = {
    callback, callback,
    callback, callback,
    callback, callback,
};

static p8_callback_arg_t args[COUNT_OF(callbacks)] = {
    -1, (P8_IND_RX_START | P8_RX_FRAME_EOM),
    P8_IND_ACK, P8_IND_NACK,
    P8_IND_TX_SUCCEEDED, P8_QRY_BUILDDATE,
};

static const p8_dispatch_table_t dt = {
    .dt_number    = COUNT_OF(callbacks),
    /* err, err, rx, ack, nack, tx, err, err, build */
    .dt_indices   = P8_DISPATCH_INDEX_TABLE(0, 0, 1, 2, 3, 4, 0, 0, 5),
    .dt_callbacks = callbacks,
};


int main(void) {
    unsigned char buffer[256], c;
    int fd, len = 0;

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    len += p8_encode(buffer + len, P8_CMD_PING,            NULL, 0); /* -> ACK */
    len += p8_encode(buffer + len, P8_QRY_BUILDDATE,       NULL, 0); /* -> BUILD */
    c = 3;
    len += p8_encode(buffer + len, P8_CMD_TX_SET_IDLETIME, &c,   1); /* -> NACK */
    /* CEC POLL src:dst = Unknown:TV */
    c = 0xf0;
    len += p8_encode(buffer + len, P8_CMD_TX_EOM,          &c,   1); /* -> NACK */

    if (len != write(fd, buffer, len)) {
        perror("write");
        exit(1);
    }

    sleep(1);                  /* NB.  Avoid repetive reads.  Ugly. */

    while ((len = read(fd, buffer, sizeof(buffer))) > 0) {
        const unsigned char *p = buffer;
        int l;
        DEBUG("Buffer: ");
        for (int i = 0; i < len; i++) {
            DEBUG(" 0x%02.2x", buffer[i]);
        }
        DEBUG("\n");
        while (len > 0) {
            l = p8_dispatch(p, len, &dt, args);
            p   += l;
            len -= l;
        }

        sleep(1);             /* NB.  Avoid repetive reads.  Ugly. */
    }

    return 0;
}
