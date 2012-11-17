/**
 * Simple ping, mainly testing encode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <proto.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_io.h>

const unsigned char P8_ACK[] = { P8_FRAME_BEGIN, P8_IND_ACK, P8_FRAME_END };

unsigned char buffer[256];

int main(void) {
    int fd, len;

    p8s_frame_t sframe = {
        .f_sta = sframe.f_buf,
        .f_end = sframe.f_buf,
    };

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    p8_encode_cmd(&sframe, P8_CMD_PING, NULL, 0);
    DEBUG("len=%ld, 0x%02.2x 0x%02.2x 0x%02.2x\n",
          sframe.f_end - sframe.f_sta,
          sframe.f_sta[0], sframe.f_sta[1], sframe.f_sta[2]);
    len = write(fd, sframe.f_buf, sframe.f_end - sframe.f_sta);
    if (len != sframe.f_end - sframe.f_sta) {
        perror("write");
        exit(1);
    }

    sleep(1);                  /* NB.  Avoid repetive reads.  Ugly. */

    len = read(fd, buffer, sizeof(buffer));

    if (len != sizeof(P8_ACK) || memcmp(buffer, P8_ACK, sizeof(P8_ACK))) {
        fprintf(stderr, "Receive failed: len=%d, 0x%02.2x 0x%02.2x 0x%02.2x\n",
                len, buffer[0], buffer[1], buffer[2]);
        exit(1);
    }
    return 0;
}
