/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Defines the CEC protocol codes.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"
#include "cec_rx.h"
#include "cec_tx.h"
#include "cec_codes.h"

/* Imitate a table through a struct */
const struct {
    cec_code_type_t abort;       /* NB.  At index zero. */

    cec_code_type_t image_view_on;
    cec_code_type_t active_source;
    cec_code_type_t set_stream_path;
    cec_code_type_t request_active_source;
    cec_code_type_t inactive_source;
    cec_code_type_t standby;

    cec_code_type_t cec_version;
    cec_code_type_t get_cec_version;
    cec_code_type_t give_physical_address;
    cec_code_type_t report_physical_address;

    cec_code_type_t menu_status;
    cec_code_type_t menu_request;
    cec_code_type_t user_control_pressed;
    cec_code_type_t user_control_released;

    cec_code_type_t give_power_status;
    cec_code_type_t report_power_status;
} _cec_code_types = {
    .abort = { },

    .image_view_on = { },
    .active_source = { .ct_tx_flags = CEC_TX_BROADCAST },
    .set_stream_path = { },
    .request_active_source = { },
    .inactive_source = { },
    .standby = { },

    .cec_version = { },
    .get_cec_version = { },
    .give_physical_address = { },
    .report_physical_address = { },

    .menu_status = { },
    .menu_request = { },
    .user_control_pressed = { },
    .user_control_released = { },

    .give_power_status = { },
    .report_power_status = { },
};

/* Make the struct look like a table. */
const cec_code_type_t *cec_code_types = &_cec_code_types.abort;

/* Indices to the table cec_code_types table. */
const unsigned char cec_code_indices[] = {
#define IOF(function) ((char *)(&_cec_code_types.function) - (char *)(&_cec_code_types))
    [CEC_IMAGE_VIEW_ON]         = IOF(image_view_on),
    [CEC_STANDBY]               = IOF(standby),
    [CEC_ACTIVE_SOURCE]         = IOF(active_source),
    [CEC_REQUEST_ACTIVE_SOURCE] = IOF(request_active_source),
#undef IOF
};

