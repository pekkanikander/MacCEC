/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC logical address assignment.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"
#include "cec_device.h"
#include "cec_rx.h"
#include "cec_tx.h"

/* NB. XXX */
unsigned short cec_logical_address_masks[] = {
#define AM(addr) (1 << (addr))
    [CEC_DEVICE_RECORDING] = (AM(1) | AM(2) | AM(9)),
    [CEC_DEVICE_TUNER]     = (AM(3) | AM(6) | AM(7) | AM(10)),
    [CEC_DEVICE_PLAYBACK]  = (AM(4) | AM(8) | AM(11)),
    [CEC_DEVICE_AUDIO]     = (AM(5)),
#undef AM
};

cec_logical_address_t
cec_logical_address_allocation(int fd, cec_device_type_t device_type,
                               cec_rx_frame_t *iframe) {
    assert_frame_invariant(iframe);

    unsigned short am = cec_logical_address_masks[device_type];
    if (am == 0) return -1;
    for (int i = 0; i < 14; i++) {
        if ((am & (1 << i))) {
            if (cec_poll(fd, CEC_MAKE_ADDRESS(i, i), iframe) == CEC_TX_NO_ACK)
                return i;
        }
    }
    return 15;                  /* Unregistered. */
}
