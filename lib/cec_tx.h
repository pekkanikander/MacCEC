/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

typedef enum cec_tx_status {
    CEC_TX_UNKNOWN = -1,
    CEC_TX_SUCCEEDED = 0,
    CEC_TX_ERROR = 1,          /* Failure reported by the lower layer */
    CEC_TX_TIMEOUT = 2,        /* Timout reported by the lower layer */
} cec_tx_status_t;

typedef struct cec_tx_buffer {
    cec_char_t      ctb_chars[CEC_MAX_PACKET_LENGTH];
    cec_count_t     ctb_char_count;
    cec_tx_status_t ctb_status;
} cec_tx_buffer_t;

extern void
cec_tx_error(cec_tx_status_t status, cec_tx_buffer_t *buffer);

