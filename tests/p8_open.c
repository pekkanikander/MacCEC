/**
 * Test p8_open.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <proto.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_io.h>

int main(void) {
    int fd;

    fd = p8_open();
    p8_close(fd);
    // NB.  This is fully legal C.  fd (as integer) doesn't change from closing it.
    if (fd < 0) {
        perror("Failed to open CEC-Adapter, check connectivity");
        exit(1);
    }

    return 0;
}
