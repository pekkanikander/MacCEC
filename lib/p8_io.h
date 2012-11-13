/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol I/O.
 *
 * This file has been explicitly placed in public domain.
 */

struct p8_io_buffer {
    p8_frame_t pib_buffer[P8_FRAME_BUFFER_LENGTH]; /* Circular buffer */
    p8_len_t   pib_read;                           /* Next character to read. */
    p8_len_t   pib_last;                           /* Last character to read + 1. */
};
/* if pib_read == pib_write, the buffer is empty. */

extern char *
p8_detect_path(char *buffer);

extern int
p8_open(void);

extern int
p8_close(int);

struct p8_dispatch_table;       /* Forward declaration. */

extern int
p8_read(int fd,  p8_frame_t *frbuf, const p8_len_t max_len, p8_io_buffer_t *pib,
        const struct p8_dispatch_table *dt, p8_callback_arg_t *cba_table);

extern int
p8_read2(int fd,  p8_frame_t *frbuf, const p8_len_t max_len, p8_io_buffer_t *pib);

extern int
p8_write(int fd, p8_frame_t *sbuf, p8_len_t slen,
         p8_io_buffer_t *pib,
         const struct p8_dispatch_table *dt, p8_callback_arg_t *cba_table);
