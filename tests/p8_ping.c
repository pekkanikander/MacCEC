/**
 * Simple ping, mainly testing encode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_io.h>

const unsigned char P8_ACK[] = { P8_FRAME_BEGIN, P8_IND_ACK, P8_FRAME_END };

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

    if (len != sizeof(P8_ACK) || memcmp(buffer, P8_ACK, sizeof(P8_ACK))) {
        fprintf(stderr, "Receive failed: len=%d, 0x%02.2x 0x%02.2x 0x%02.2x",
                len, buffer[0], buffer[1], buffer[2]);
        exit(1);
    }
    return 0;
}
