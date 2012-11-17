/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol I/O.
 *
 * This file has been explicitly placed in public domain.
 */

extern char *
p8_detect_path(char *buffer);

extern int
p8_open(void);

extern int
p8_close(int);

extern int
p8_read_and_dispatch(int fd,  p8s_frame_t *iframe,
        const struct proto_dispatch_table *dt, proto_callback_arg_t *cba_table);

extern int
p8_read(int fd, p8s_frame_t *iframe);


extern int
p8_write(int fd, p8s_frame_t *oframe,
         const struct proto_dispatch_table *dt, proto_callback_arg_t *cba_table);
