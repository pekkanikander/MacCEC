/**
 * Second ping, testing encode and decode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_io.h>

static int acked = 0;

void callback(p8_code_t code,
              const p8_param_t *params, p8_len_t param_len,
              p8_callback_arg_t dummy) {
    assert(code == P8_IND_ACK);
    acked = 1;
}

int main(void) {
    unsigned char buffer[P8_FRAME_BUFFER_LENGTH];
    int fd, len;

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    len = p8_encode(buffer, P8_CMD_PING, NULL, 0);
    if (len != write(fd, buffer, len)) {
        perror("write");
        exit(1);
    }

    sleep(1);                  /* NB.  Avoid repetive reads.  Ugly. */

    len = read(fd, buffer, P8_FRAME_BUFFER_LENGTH);

    p8_callback_arg_t cba;
    const unsigned char *p = buffer;
    int l;

    while ((l = p8_decode(p, len, callback, cba)) > 0) {
        p   += l;
        len -= l;
    }

    if (!acked) {
        fprintf(stderr, "No ack received!\n");
        exit (1);
    }

    return 0;
}
