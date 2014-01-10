/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol logical_device.
 *
 * This file has been explicitly placed in public domain.
 */

typedef proto_frame_t cec_rx_frame_t;

typedef int (*cec_poll_callback_t)(cec_address_byte_t address_byte);

typedef struct cec_device {
    int                     d_fd;
    cec_device_type_t       d_type;      /* NB.  Redundant. */
    cec_logical_address_t   d_address;
    proto_dispatch_table_t *d_dt;        /* NB.  May change depending on the receiver state. */
    cec_poll_callback_t     d_poll;
} cec_device_t;

typedef struct cec_rx_dispatch_table {
    cec_device_t   *rdt_devices[CEC_LOGICAL_ADDRESS_NUMBER];
} cec_rx_dispatch_table_t;

typedef struct cec_callback_arg {
    cec_logical_address_t  *cba_receiver; /* Packet receiver */
} cec_callback_arg_t;

#define assert_device_invariant(d) /* XXX */

extern cec_logical_address_t
cec_logical_address_allocation(int fd, cec_device_type_t device_type, cec_rx_frame_t *iframe);

extern int
cec_command(cec_device_t *device,
            enum cec_code code,
            proto_char_t *params, proto_len_t len);

extern int
cec_device_initialize(
    cec_device_t *d,
    cec_rx_dispatch_table_t *dt,
    int fd,
    cec_physical_address_t physical_address,
    cec_device_type_t device_type);
