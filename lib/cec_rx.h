/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Implements the CEC protocol device independent parts.
 *
 * This file has been explicitly placed in public domain.
 */

/**
 * CEC HL msg receiving state machine.
 */
typedef enum cec_rx_frame_state {
    CEC_RX_EMPTY,          /* Empty buffer */
    CEC_RX_PROGRESS,       /* Progressing */
    CEC_RX_EOM,            /* EOM recieved*/
    CEC_RX_OVF,            /* Overflow, too long message */
    CEC_RX_ERROR,          /* Failure reported by lower layer */
} cec_rx_frame_state_t;

typedef proto_frame_t cec_rx_frame_t;

extern void
cec_rx_char_cb(cec_char_t c, cec_flags_t f, cec_rx_frame_t *frame);

extern int
cec_rx_error(cec_rx_frame_state_t state, cec_char_t c, cec_flags_t f, cec_rx_frame_t *frame);

/*
 * NB.  No need to explicit cec_dispatch.  Can use proto_dispatch directly.
 */
extern int
cec_rx_msg(int fd, cec_rx_frame_t *iframe);

