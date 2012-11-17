/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

extern int
p8_cec_rx_callback(proto_char_t code,
                   const p8_frame_t *params,
                   proto_callback_arg_t cba);

extern int
p8_cec_rx_error_cb(proto_char_t code,
                   const p8_frame_t *params,
                   proto_callback_arg_t cba);

/*
 * Internal routine to receive a cec frame until EOM.
 */

extern int
p8_cec_rx(int fd, cec_rx_frame_t *frame, p8_io_buffer_t *pib);

