/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

/* Forward declarations */
typedef enum p8_code p8_code_t;
typedef struct proto_frame p8_frame_t;

#if 1
#define DEBUG(...) printf(__VA_ARGS__)
#include <stdio.h>
#else
#define DEBUG(...)
#endif

extern int
p8_command(int fd, enum p8_code code, proto_char_t *params, proto_len_t len,
           proto_frame_t *iframe);

extern int
p8_command1(int fd, enum p8_code code, proto_char_t param,
            proto_frame_t *iframe);

#define p8_command_ping(fd, iframe)            p8_command (fd, P8_CMD_PING, NULL, 0, iframe)
#define p8_command_set_controlled(fd, iframe)  p8_command1(fd, P8_SET_CONTROLLED, 1, iframe)
