/**
 * Test CEC logical device initialise.
 *
 * Placed in public domain.
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
#include <cec_logical_device.h>

#define DEBUG(...) printf(__VA_ARGS__)

int
main(int ac, char **av) {
    cec_logical_device_t device;
    cec_device_type_t type = CEC_DEVICE_PLAYBACK;
    int src = 0xf, dst = 0x0;
    int fd;

    switch (ac) {
    case 2:
        type = atoi(av[1]);
        /* Fallthrough */
    case 1:
        break;
    default:
        fprintf(stderr, "Usage: %s [type]\n", av[0]);
        exit (1);
    }

    if ((fd = cec_open()) < 0) {
        perror("open");
        exit(1);
    }

    cec_physical_address_t physical = cec_get_physical_address(fd);

    return cec_logical_device_initialize(
        &device,
        0 /* XXX */,
        fd,
        physical,
        type);
}
