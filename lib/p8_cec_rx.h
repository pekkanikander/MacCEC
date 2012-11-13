/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

extern void
p8_cec_rx_callback(p8_code_t code,
                   const p8_param_t *params, p8_len_t params_len,
                   p8_callback_arg_t cba);

extern void
p8_cec_rx_error_cb(p8_code_t code,
                   const p8_param_t *params, p8_len_t params_len,
                   p8_callback_arg_t cba);

/*
 * Internal routine to receive a cec frame until EOM.
 */

extern cec_rx_status_t
p8_cec_rx(int fd, cec_rx_buffer_t *buffer, p8_io_buffer_t *pib);

