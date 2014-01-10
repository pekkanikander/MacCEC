/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

#define CEC_MAX_PACKET_LENGTH 16
#define CEC_MAX_PARAMS_LENGTH 14

typedef unsigned char cec_char_t, cec_header_t;

typedef enum cec_flags {
    CEC_ACK   = 0x01,
    CEC_EOM   = 0x02,
    CEC_START = 0x04,
} cec_flags_t;

typedef enum cec_signal_free_time {
    CEC_SIGNAL_FREE_LARGE = 7,
    CEC_SIGNAL_FREE_NEW = 5,
    CEC_SIGNAL_FREE_RETRY = 3,
} cec_signal_free_time_t;

typedef enum cec_device_type {
    CEC_DEVICE_TV = 0,
    CEC_DEVICE_RECORDING = 1,
    CEC_DEVICE_TUNER = 3,
    CEC_DEVICE_PLAYBACK = 4,
    CEC_DEVICE_AUDIO = 5,
} cec_device_type_t;

typedef unsigned char cec_count_t;

#define CEC_LOGICAL_ADDRESS_MASK   0x0f
#define CEC_LOGICAL_ADDRESS_NUMBER 16

typedef unsigned char cec_logical_address_t;

typedef unsigned short cec_physical_address_t;

/* Forward declarations */
enum cec_code;
enum cec_tx_status;

#define CEC_MAKE_ADDRESS(src, dst) \
    ((((src) & CEC_LOGICAL_ADDRESS_MASK) << 4) | ((dst) & CEC_LOGICAL_ADDRESS_MASK))
#define CEC_ADDR_DST(addr) ((addr) & CEC_LOGICAL_ADDRESS_MASK)

typedef enum cec_tx_status {
    CEC_TX_ERROR     = -2,    /* Failure reported by the lower layer */
    CEC_TX_UNKNOWN   = -1,
    CEC_TX_SUCCEEDED = 0,
    CEC_TX_NO_ACK    = 1,     /* Not acked  */
    CEC_TX_TIMEOUT   = 2,     /* Timout reported by the lower layer */
} cec_tx_status_t;

typedef unsigned char cec_address_byte_t;

extern int
cec_open(void);

extern cec_physical_address_t
cec_get_physical_address(int fd);

extern enum cec_tx_status
cec_poll(int fd,
         cec_header_t addr,
         struct proto_frame *iframe);

