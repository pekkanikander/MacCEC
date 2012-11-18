/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

#define CEC_MAX_PACKET_LENGTH 16
#define CEC_MAX_PARAMS_LENGTH 14

typedef unsigned char cec_char_t, cec_header_t;

typedef enum cec_flags {
    CEC_ACK   = 0x01,
    CEC_EOM   = 0x02,
    CEC_START = 0x04,
} cec_flags_t;

typedef enum cec_signal_free_time {
    CEC_SIGNAL_FREE_LARGE = 7,
    CEC_SIGNAL_FREE_NEW = 5,
    CEC_SIGNAL_FREE_RETRY = 3,
} cec_signal_free_time_t;

typedef unsigned char cec_count_t;
/* Forward declarations */
enum cec_code;

#define CEC_MAKE_ADDRESS(src, dst) ((((src) & 0xf) << 4) | ((dst) & 0xf))

extern int
cec_open(void);

extern int
cec_command(int fd, 
            cec_header_t addr,
            enum cec_code code, 
            proto_char_t *params, proto_len_t len, 
            struct proto_frame *iframe);

extern int
cec_command_poll(int fd, 
                 cec_header_t addr,
                 struct proto_frame *iframe);
