/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

extern void
p8_cec_tx_callback(p8_code_t code,
                   const p8_param_t *params, p8_len_t params_len,
                   p8_callback_arg_t cba);

extern void
p8_cec_tx_error_cb(p8_code_t code,
                   const p8_param_t *params, p8_len_t params_len,
                   p8_callback_arg_t cba);

/*
 * Internal routine to transmit a CEC frame.
 */

extern cec_tx_status_t
p8_cec_tx(int fd, unsigned char idletime,
          cec_tx_buffer_t *ctb, cec_rx_buffer_t *crb, p8_io_buffer_t *pib);

