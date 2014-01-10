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
