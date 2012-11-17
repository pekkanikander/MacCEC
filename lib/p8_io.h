/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol I/O.
 *
 * This file has been explicitly placed in public domain.
 */

struct p8_io_buffer {
    proto_char_t pib_buffer[256]; /* Circular buffer */
    proto_len_t  pib_read;        /* Next character to read. */
    proto_len_t  pib_last;        /* Last character to read + 1. */
};
/* if pib_read == pib_write, the buffer is empty. */

extern char *
p8_detect_path(char *buffer);

extern int
p8_open(void);

extern int
p8_close(int);

extern int
p8_read_and_dispatch(int fd,  p8s_frame_t *iframe,
        p8_io_buffer_t *pib,
        const struct proto_dispatch_table *dt, proto_callback_arg_t *cba_table);

extern int
p8_read(int fd, p8s_frame_t *iframe,
         p8_io_buffer_t *pib);

extern int
p8_write(int fd, p8s_frame_t *oframe,
         p8_io_buffer_t *pib,
         const struct proto_dispatch_table *dt, proto_callback_arg_t *cba_table);
