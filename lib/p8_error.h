/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol error reporting.
 *
 * This file has been explicitly placed in public domain.
 */

extern int
p8_error(proto_char_t code,
         const p8_frame_t *params,
         proto_callback_arg_t cb_arg);

