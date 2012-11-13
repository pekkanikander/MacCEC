/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol dispatching.
 *
 * This file has been explicitly placed in public domain.
 */

/*
 * Callbacks functions in standard dispatch tables, see p8_dispatch.c
 */
typedef unsigned char p8_dispatch_index_t;

#define P8_DISPATCH_INDEX_TABLE(proto_err, rx_err, rx, ack, nack, tx, tx_err, fw_reply, build_reply) \
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
        build_reply, /* QRY_BUILDATE */         \
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
}

#define P8_DISPATCH_DEFAULT_INDEX_TABLE_LENGTH 9

#define P8_DISPATCH_DEFAULT_INDEX_TABLE \
    P8_DISPATCH_INDEX_TABLE(0, 1, 2, 3, 4, 5, 6, 7, 8)

#define P8_DISPATCH_DEFAULT_VECTOR \
    p8_error, p8_cec_rx_error_cb, p8_cec_rx_callback

/*
 * P8 dispatchs
 *
 * CMD -> p8_dispatch_idx -> p8_dispatch_table -> callback
 */

typedef struct p8_dispatch_table {
    unsigned char              dt_number;      /* Number of callbacks in table */
    const p8_dispatch_index_t  dt_indices[P8_CODE_NUMBER]; /* Map p8 codes to cb indices */
    const p8_callback_t *      dt_callbacks;   /* The callbacks themselves */
} p8_dispatch_table_t;


extern int
p8_dispatch(const unsigned char *frbuf, p8_len_t len,
            const p8_dispatch_table_t *table, p8_callback_arg_t *cb_arg_table);
