/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

extern int
p8_cec_tx_callback(proto_char_t code,
                   const p8_frame_t *frame,
                   proto_callback_arg_t cba);

extern int
p8_cec_tx_error_cb(proto_char_t code,
                   const p8_frame_t *frame,
                   proto_callback_arg_t cba);

/*
 * Internal routine to transmit a CEC frame.
 */

extern cec_tx_status_t
p8_cec_tx(int fd, cec_signal_free_time_t idletime,
          cec_tx_frame_t *oframe, cec_rx_frame_t *iframe);

