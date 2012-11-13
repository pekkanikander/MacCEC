/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol I/O.
 *
 * This file has been explicitly placed in public domain.
 */

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"
#include "p8_io.h"

#include <unistd.h>
#include <assert.h>

int
p8_read(int fd,  p8_frame_t *frbuf, const p8_len_t max_len, p8_io_buffer_t *pib,
        const struct p8_dispatch_table *dt, p8_callback_arg_t *cba_table) {
    int len = 0;

    /* Read at least one P8 frame.  */
    len = p8_read2(fd, frbuf, max_len, pib);
    if (len <= 0) {
        return len;
    }

    int l = len;
    p8_frame_t *p = frbuf;

    while (l > 0) {
        int consumed = p8_dispatch(p, l, dt, cba_table);
        l -= consumed;
        p += consumed;
    }

    assert(l == 0);
    assert(p == frbuf + len);

    return len;
}

int
p8_write(int fd, p8_frame_t *sbuf, p8_len_t slen,
         p8_io_buffer_t *pib,
         const struct p8_dispatch_table *dt, p8_callback_arg_t *cba_table) {

    p8_frame_t rbuf[P8_FRAME_BUFFER_LENGTH];
    int rlen, rv;

    if (slen != write(fd, sbuf, slen)) {
        return -1;
    }

    rlen = p8_read(fd, rbuf, sizeof(rbuf), pib, dt, cba_table);
    if (rlen <= 0) {
        return -1;
    }

    return slen;
}

