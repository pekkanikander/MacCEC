/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol logical_device.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"
#include "cec_device.h"
#include "cec_rx.h"

int
cec_device_initialize(
    cec_device_t *d,
    cec_rx_dispatch_table_t *dt,
    int fd,
    cec_physical_address_t physical_address,
    cec_device_type_t device_type) {

    PROTO_FRAME(cec_rx_frame_t, iframe, 0, 0);

    d->d_fd      = fd;
    d->d_type    = device_type;
    d->d_address
        = cec_logical_address_allocation(fd, device_type, iframe);

    if (!proto_frame_is_empty(iframe))
        cec_dispatch(iframe, dt);

    assert_logical_device_invariant(d);

    return 0;
}
