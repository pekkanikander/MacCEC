/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol dispatching.
 *
 * This file has been explicitly placed in public domain.
 */

/*
 * Dispatch tables used for the P8 protocol.
 */

#define P8_DISPATCH_INDEX_TABLE(proto_err, rx_err, rx, ack, nack, tx, tx_err, fw_reply) \
    { \
        proto_err, /* NONE */                   \
        proto_err, /* CMD_PING */               \
        rx_err,    /* IND_ERROR_TIMEOUT */      \
        rx_err,    /* IND_ERROR_HIGH */         \
        rx_err,    /* IND_ERROR_LOW */          \
        rx,        /* IND_RX_START */           \
        rx,        /* IND_RX_NEXT */            \
        rx_err,    /* INX_RX_FAILED */          \
                                                \
        ack,       /* IND_ACK */                \
        nack,      /* IND_NACK */               \
        proto_err, /* CMD_SET_ACK_MASK */       \
        proto_err, /* CMD_TX           */       \
        proto_err, /* CMD_TX_EOM */             \
        proto_err, /* CMD_TX_SET_IDLETIME */    \
        proto_err, /* CMD_TX_SET_ACK_POL */     \
        proto_err, /* CMD_TX_SET_TIMEOUT */     \
                                                \
        tx,        /* IND_TX_SUCCEEDED */       \
        tx_err,    /* IND_TX_FAIL_LINE */       \
        tx_err,    /* IND_TX_FAIL_ACK */        \
        tx_err,    /* IND_TX_TIMEOUT_DATA */    \
        tx_err,    /* IND_TX_TIMEOUT_LINE */    \
        fw_reply,  /* QRY_FIRMWARE_VERS */      \
        proto_err, /* CMD_BOOT */               \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* CMD_SET_CONTROLLED */     \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
                                                \
        proto_err, /* 0x40 */                   \
        proto_err, /* CMD_PING */               \
        rx_err,    /* IND_ERROR_TIMEOUT */      \
        rx_err,    /* IND_ERROR_HIGH */         \
        rx_err,    /* IND_ERROR_LOW */          \
        rx,        /* IND_RX_START */           \
        rx,        /* IND_RX_NEXT */            \
        rx_err,    /* INX_RX_FAILED */          \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
                                                \
        proto_err, /* 0x80 */                   \
        proto_err, /* CMD_PING */               \
        rx_err,    /* IND_ERROR_TIMEOUT */      \
        rx_err,    /* IND_ERROR_HIGH */         \
        rx_err,    /* IND_ERROR_LOW */          \
        rx,        /* IND_RX_START */           \
        rx,        /* IND_RX_NEXT */            \
        rx_err,    /* INX_RX_FAILED */          \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
                                                \
        proto_err, /* 0xC0 */                   \
        proto_err, /* CMD_PING */               \
        rx_err,    /* IND_ERROR_TIMEOUT */      \
        rx_err,    /* IND_ERROR_HIGH */         \
        rx_err,    /* IND_ERROR_LOW */          \
        rx,        /* IND_RX_START */           \
        rx,        /* IND_RX_NEXT */            \
        rx_err,    /* INX_RX_FAILED */          \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
                                                \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
        proto_err, /* TBD */                    \
}

#define P8_DISPATCH_DEFAULT_INDEX_TABLE_LENGTH 8

#define P8_DISPATCH_DEFAULT_INDEX_TABLE \
    P8_DISPATCH_INDEX_TABLE(0, 1, 2, 3, 4, 5, 6, 7)

#define P8_DISPATCH_DEFAULT_VECTOR \
    p8_error, p8_cec_rx_error_cb, p8_cec_rx_callback


extern int
p8_dispatch(p8s_frame_t *iframe,
            const proto_dispatch_table_t *table,
            proto_callback_arg_t *cb_arg_table);
