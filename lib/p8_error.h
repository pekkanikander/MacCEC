/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol error reporting.
 *
 * This file has been explicitly placed in public domain.
 */

extern void
p8_error(p8_code_t code,
         const p8_param_t *params, p8_len_t param_len,
         p8_callback_arg_t cb_arg);

