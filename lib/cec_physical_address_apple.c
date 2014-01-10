/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Returns the physical address.
 *
 * This file has been explicitly placed in public domain.
 */

#include <assert.h>

#include "proto.h"

#include "cec.h"

cec_physical_address_t
cec_get_physical_address(int fd) {
    return 0x1000;              /* FAKE */
}
