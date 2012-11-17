/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

/* Forward declarations */
typedef enum p8_code p8_code_t;
typedef struct proto_frame p8_frame_t;
typedef struct p8_io_buffer p8_io_buffer_t;;

#if 1
#define DEBUG(...) printf(__VA_ARGS__)
#include <stdio.h>
#else
#define DEBUG(...)
#endif

extern int
p8_command(int fd, enum p8_code code, proto_char_t *params, proto_len_t len,
           p8_io_buffer_t *pib, proto_frame_t *iframe);

extern int
p8_command1(int fd, enum p8_code code, proto_char_t param,
            p8_io_buffer_t *pib, proto_frame_t *iframe);

#define p8_command_ping(fd, pib, iframe)            p8_command (fd, P8_CMD_PING, NULL, 0, pib, iframe)

#define p8_command_set_controlled(fd, pib, iframe)  p8_command1(fd, P8_SET_CONTROLLED, 1, pib, iframe)
