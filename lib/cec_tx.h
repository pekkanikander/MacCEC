/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

typedef enum cec_tx_flags {
    CEC_TX_NONE      = 0x00,
    CEC_TX_BROADCAST = 0x01,
} cec_tx_flags_t;

typedef proto_frame_t cec_tx_frame_t;

extern void
cec_tx_error(cec_tx_status_t status, cec_tx_frame_t *frame);

/*
 * Internal routine to transmit a CEC frame.
 */

extern cec_tx_status_t
p8_cec_tx(int fd, cec_signal_free_time_t idletime, cec_tx_flags_t flags,
          cec_tx_frame_t *oframe, cec_rx_frame_t *iframe);
