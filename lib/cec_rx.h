/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

typedef enum cec_rx_status {
    CEC_RX_OK  = 0,            /* The character was stored ok */
    CEC_RX_EOM = 1,            /* EOM was already recieved, char dropped */
    CEC_RX_OVF = 2,            /* Overflow, too long message */
    CEC_RX_ERROR = 3,          /* Failure reported by lower layer */
} cec_rx_status_t;

typedef struct cec_rx_buffer {
    cec_char_t      crb_chars[CEC_MAX_PACKET_LENGTH];
    cec_flags_t     crb_flags[CEC_MAX_PACKET_LENGTH];
    cec_count_t     crb_char_count;
    cec_rx_status_t crb_status;
} cec_rx_buffer_t;

extern cec_rx_status_t
cec_rx_char_cb(cec_char_t c, cec_flags_t f, cec_rx_buffer_t *buffer);

extern void
cec_rx_error(cec_rx_status_t status, cec_char_t c, cec_flags_t f, cec_rx_buffer_t *buffer);

