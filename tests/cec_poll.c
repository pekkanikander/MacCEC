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

#include <cec.h>
#include <cec_rx.h>

#define DEBUG(...) printf(__VA_ARGS__)

int
main(int ac, char **av) {
    int src = 0xf, dst = 0x0;
    int fd;

    switch (ac) {
    case 3:
        dst = atoi(av[2]);
        /* Fallthrough */
    case 2:
        src = atoi(av[1]);
        /* Fallthrough */
    case 1:
        break;
    default:
        fprintf(stderr, "Usage: %s [src] [dst]\n", av[0]);
        exit (1);
    }

    if ((fd = cec_open()) < 0) {
        perror("open");
        exit(1);
    }

    cec_rx_frame_t iframe = {
        .f_sta = iframe.f_buf,
        .f_end = iframe.f_buf,
    };
    return cec_command_poll(fd, CEC_MAKE_ADDRESS(src, dst), &iframe);
}
