/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol codes.
 *
 * This file has been explicitly placed in public domain.
 */

/*
 * P8 frame code points:
 *  P8_FRAME_START code parameters P8_FRAME_END
 */

enum p8_code {
    P8_CMD_NONE            = 0x00,
    P8_CMD_PING            = 0x01,

    P8_IND_ERROR_TIMEOUT   = 0x02, /* TBD.  Bit error? */
    P8_IND_ERROR_HIGH      = 0x03, /* TBD.  Bit error? */
    P8_IND_ERROR_LOW       = 0x04, /* TBD.  Bit error? */

    P8_IND_RX_START        = 0x05, /* First byte in a CEC frame, usually ACKed (0x45) */
    P8_IND_RX_NEXT         = 0x06, /* Next byte in a CEC frame, usually ACKed (0x46) */
                                   /* and maybe with an EOM (0xc6) */
    P8_IND_RX_FAILED       = 0x07, /* TBD.  Frame error? */

    P8_IND_ACK             = 0x08, /*  ACK previous command */
    P8_IND_NACK            = 0x09, /* NACK previous command */
    P8_CMD_SET_ACK_MASK    = 0x0A, /* TBD. */

    P8_CMD_TX              = 0x0B, /* Transmit a CEC byte */
    P8_CMD_TX_EOM          = 0x0C, /* Transmit a CEC byte, and EOM */

    P8_CMD_TX_SET_IDLE     = 0x0D, /* TBD.  Set CEC TX Signal Free Time?= */
    P8_CMD_TX_SET_ACK_P    = 0x0E, /* TBD.  Set CEC TX ACK polarity (for multicast/unicast) */
    P8_CMD_TX_SET_TIMEOUT  = 0x0F, /* TBD.  Set CEC TX reply timeout?? */

    P8_IND_TX_SUCCEEDED    = 0x10, /* Transmit succeeded */
    P8_IND_TX_FAIL_LINE    = 0x11, /* Transmit failed.  TBD.  Bit error? */
    P8_IND_TX_FAIL_ACK     = 0x12, /* Transmit NACKed */
    P8_IND_TX_TIMEOUT_D    = 0x13, /* TBD */
    P8_IND_TX_TIMEOUT_L    = 0x14, /* TBD */

    P8_QRY_FIRMWARE        = 0x15, /* Query/reply firmware version */
    P8_SET_CONTROLLED      = 0x18, /* Host-side control mode. */
};

#define P8_CODE_NUMBER   64
#define P8_CODE_MASK   0x3F

/*
 * Flags available at RX_START, RX_NEXT, and maybe RX_FAILED
 */

#define P8_RX_FRAME_ACK 0x40
#define P8_RX_FRAME_EOM 0x80

/* Message code types */
#define P8_CT_NONE  0x00        /* Undefined type ignored */
#define P8_CT_H2A   0x01        /* Host to adapter */
#define P8_CT_A2H   0x02        /* Adapter to host */
#define P8_CT_BIDIR 0x03        /* Bidirectional */
#define P8_CT_TMASK 0x03        /* Type mask */

/* Expect reply type */
#define P8_CT_ACKED 0x08        /* Acked/Nacked */
#define P8_CT_TXIND 0x10        /* Transmit success/failure indicated */
#define P8_CT_REPLD 0x20        /* Replied with the same code  */

#define P8_CT_H2A_ACKED ( P8_CT_H2A   | P8_CT_ACKED )
#define P8_CT_H2A_TXEOM ( P8_CT_H2A   | P8_CT_ACKED  | P8_CT_TXIND )
#define P8_CT_QUERY     ( P8_CT_BIDIR | P8_CT_REPLD )

typedef struct p8_code_type {
    unsigned char ct_type;
    unsigned char ct_h2a_min_plen; /* Host-to-adapter parameters min length */
    unsigned char ct_h2a_max_plen; /* Host-to-adapter parameters max length */
    unsigned char ct_a2h_min_plen; /* Adapter-to-host parameters min length */
    unsigned char ct_a2h_max_plen; /* Adapter-to-host parameters max length */
} p8_code_type_t;

extern const p8_code_type_t p8_code_types[];
extern const unsigned int   P8_CODE_TYPE_COUNT; /* Number of entries in p8_code_types */

#define assert_code_tx_invariant(code, len)                \
    assert((code) < P8_CODE_TYPE_COUNT),                   \
    assert(p8_code_types[(code)].ct_type & P8_CT_H2A),     \
    assert((len) >= p8_code_types[(code)].ct_h2a_min_plen),\
    assert((len) <= p8_code_types[(code)].ct_h2a_max_plen)

#define assert_code_rx_invariant(code, frame)              \
    assert((code) < P8_CODE_TYPE_COUNT),                   \
    assert(p8_code_types[(code)].ct_type & P8_CT_A2H),     \
    assert(((frame)->f_end - (frame)->f_sta) >= p8_code_types[(code)].ct_a2h_min_plen), \
    assert(((frame)->f_end - (frame)->f_sta) <= p8_code_types[(code)].ct_a2h_max_plen)
