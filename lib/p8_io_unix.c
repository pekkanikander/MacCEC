/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol I/O Unix specific parts.
 *
 * This file has been explicitly placed in public domain.
 */

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <sys/errno.h>
#include <sys/param.h>          /* For MAXPATHLEN */

#include "proto.h"
#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_io.h"

/**
 * Opens a communication channel with the P8 CEC-USB adapter.
 *
 * @Returns an open file descriptor, or -1 on error.
 */

int
p8_open(void) {
    char path[MAXPATHLEN];

    if (!p8_detect_path(path)) {
        errno = ENOENT;
        return -1;
    }

    return open(path, O_RDWR);
}

/**
 * Closes a communication channel with the P8 CEC-USB adapter.
 *
 * @Returns 0 on success and -1 on error.
 */
int
p8_close(int fd) {
    return close(fd);
}

/**
 * Reads one or more P8 messages to the end of iframe.
 * Does not block.
 *
 * @Returns 0 if succeeded.
 * A return value < 0 means error, see errno.
 * May return an incomplete frame on read errors.
 */
int
p8_read(int fd, p8s_frame_t *f) {

    const proto_char_t *max_end  = f->f_buf + COUNT_OF(f->f_buf);

    /* If the frame is empty, reset it */
    assert_frame_invariant(f);
    if (proto_frame_is_empty(f)) {
        f->f_sta = f->f_end = f->f_buf;
    } /* NB. 'else' not needed here.  Smart compiler
         will add one. */

    /* If the frame buffer is getting close to the end, pack it. */
    if (f->f_end > f->f_buf + COUNT_OF(f->f_buf) / 2 &&
        f->f_sta > f->f_buf + COUNT_OF(f->f_buf) / 4) {
        memmove(f->f_buf, f->f_sta, f->f_end - f->f_sta);
        f->f_end -= f->f_sta - f->f_buf;
        f->f_sta = f->f_buf;
    }
    assert_frame_invariant(f);

    assert(f->f_end < max_end);            /* Don't allow full buffer. */

    for (;;) {
        /* Invariant */
        assert_frame_invariant(f);
        assert(f->f_end < max_end);

        /* Read a bunch of data */
        //DEBUG("p8_io_unix: Reading...\n");
        int rv = read(fd, f->f_end, max_end - f->f_end);
        //DEBUG("p8_io_unix: Reading...got %d.\n", rv);
        if (rv <= 0) {
            return rv;
        }
        f->f_end += rv;

        assert_frame_invariant(f);
        assert_frame_not_empty(f);

        /* Return if the buffer is full or we got a P8_FRAME_END. */
        if (f->f_end == max_end || f->f_end[-1] == P8_FRAME_END) {
            return 0;
        }
    }
}

