/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol dispatching.
 *
 * This file has been explicitly placed in public domain.
 */

#include "p8.h"
#include "p8_codes.h"
#include "p8_frame.h"
#include "p8_dispatch.h"

struct p8_dispatch_callback_arg {
    const p8_dispatch_table_t *dtcba_dt;        /* Dispatch table */
    p8_callback_arg_t         *dtcba_cba_table; /* Arguments, dtcba_dt->dt_number */
};

static void
dispatch_callback(p8_code_t code,
                  const p8_param_t *params, p8_len_t len,
                  p8_callback_arg_t cba) {
    const struct p8_dispatch_callback_arg *dtcba = cba.cba_dispatch;
    const p8_dispatch_table_t *dt = dtcba->dtcba_dt;
    p8_callback_arg_t *cba_table  = dtcba->dtcba_cba_table;
    p8_dispatch_index_t index = dt->dt_indices[code & P8_CODE_MASK];
    if (index >= dt->dt_number) {
        return;
    }

    dt->dt_callbacks[index](code, params, len, cba_table[index]); /* NB.  Tail */
}

int
p8_dispatch(const p8_frame_t *frbuf, p8_len_t len,
            const p8_dispatch_table_t *dt, p8_callback_arg_t *cba_table) {
    struct p8_dispatch_callback_arg dtcba = {
        .dtcba_dt        = dt,
        .dtcba_cba_table = cba_table,
    };
    p8_callback_arg_t cba = {
        .cba_dispatch = &dtcba,
    };
    return p8_decode(frbuf, len, dispatch_callback, cba); /* NB.  Tail */
}
