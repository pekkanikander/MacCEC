/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol error reporting.
 *
 * This file has been explicitly placed in public domain.
 */

#include <stdio.h>

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_error.h"

extern void
p8_error(p8_code_t code,
         const p8_param_t *params, p8_len_t param_len,
         p8_callback_arg_t cb_arg) {
    fprintf(stderr, "P8 PROTOCOL ERROR: code=0x%02.2x, param_len=%d", code, param_len);
    for (int i = 0; i < param_len; i++)
        fprintf(stderr, ", 0x%02.2x", params[i]);
    fprintf(stderr, "; cb_arg=0x%p\n", cb_arg.cba_intp);
}

