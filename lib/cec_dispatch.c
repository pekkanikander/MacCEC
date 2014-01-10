/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Dispatch received messages.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"
#include "cec_rx.h"

static const proto_callback_t cec_XXX_callbacks[CEC_LOGICAL_DEVICE_NUMBER] = {
};

extern int
cec_dispatch(
    cec_rx_frame_t *cec_rx_frame,
    cec_rx_dispatch_table_t *rdt) {

    cec_address_byte_t address_byte = proto_frame_getc(cec_rx_frame);
    cec_device_t *receiver = rtd->rtd_devices[CEC_ADDR_DST(address_byte)];
    if (proto_frame_is_empty(cec_rx_frame)) {
        return receiver->d_poll(address_byte);
    }
    cec_code_t code = proto_frame_getc(cec_rx_frame);

    cec_callback_arg_t cba = {
        .cba_receiver = receiver;
        .cba_code     = code;
    };

    proto_dispatch(code, cec_rx_frame, receiver->d_dt, &cba);

    return 0;
}

