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

typedef proto_frame_t cec_tx_frame_t;

extern void
cec_tx_error(cec_tx_status_t status, cec_tx_frame_t *frame);

extern int
p8_cec_tx(int fd, unsigned char idletime,
          cec_tx_frame_t *cec_oframe, cec_rx_frame_t *cec_iframe);
