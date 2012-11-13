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

#include <sys/errno.h>
#include <sys/param.h>          /* For MAXPATHLEN */

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_io.h"

#include <assert.h>

/*
 * XXX: Revise the non-blocking operations.
 *      Currently it looks like that we are non-block
 *      for no reason, but that needs to be re-evaluated
 *      once we get threads support there.
 */

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

    return open(path, O_RDWR | O_NONBLOCK);
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
 * Reads one or more P8 messages.  Does not block.
 *
 * @Returns the number of bytes read, maybe zero.
 * A return value < 0 means error, see errno.
 * May return an incomplete frame on read errors.
 */
int
p8_read2(int fd, p8_frame_t *frbuf, const p8_len_t max_len, p8_io_buffer_t *pib) {
    int len = 0;

    assert(max_len > 0);            /* Don't allow zero reads. */
    for (;;) {
        /* Invariant */
        assert(len <= max_len);
        assert(pib->pib_read <= pib->pib_last);
        assert(pib->pib_last <= sizeof(pib->pib_buffer));

        /*
         * Copy data from the read buffer, if any.
         */
        while (len < max_len && pib->pib_read < pib->pib_last) {
            p8_len_t cc = pib->pib_last - pib->pib_read;

            if (cc > max_len - len)
                cc = max_len - len;
            assert(cc > 0 && cc <= max_len - len);
            memcpy(frbuf + len, pib->pib_buffer + pib->pib_read, cc);
            pib->pib_read += cc;
            len          += cc;
        }
        /* Now the read buffer must be empty or the return buffer full. */
        assert(pib->pib_read == pib->pib_last || len == max_len);

        assert(len <= max_len);
        assert(pib->pib_read <= pib->pib_last);
        assert(pib->pib_last <= sizeof(pib->pib_buffer));

        /* Return if the return buffer is full or we have a full frame. */
        if (len == max_len || (len > 0 && frbuf[len-1] == P8_FRAME_END)) {
            return len;
        }

        assert(pib->pib_read == pib->pib_last);

        /* Read more data. The read buffer must be empty here, so reset it. */
        pib->pib_read = pib->pib_last = 0;
        {
            int cc, rv;

            /* XXX MAY BUSY LOOP. */
            do {
                rv = read(fd, pib->pib_buffer, sizeof(pib->pib_buffer));
            } while (rv == -1 && errno == EAGAIN);
            if (rv <= 0) {
                /* Didn't get any data. */
                /* Return error if no data yet. */
                if (len == 0)
                    return rv;
                /* Return anyway.  Hope we will succeed next round. */
                return len;
            }
            pib->pib_last = rv;
        }
    }
}

