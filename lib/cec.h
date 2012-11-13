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

typedef unsigned char cec_char_t;

typedef enum cec_flags {
    CEC_ACK   = 0x01,
    CEC_EOM   = 0x02,
    CEC_START = 0x04,
} cec_flags_t;

typedef unsigned char cec_count_t;

