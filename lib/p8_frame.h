/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol framing.
 *
 * This file has been explicitly placed in public domain.
 */

/*
 * P8 framing
 */

enum p8_framing {
    P8_FRAME_ESC   = 0xfd,
    P8_FRAME_END   = 0xfe,
    P8_FRAME_BEGIN = 0xff,
};
#define P8_FRAME_ESC_OFFSET (P8_FRAME_BEGIN - P8_FRAME_ESC + 1)

enum p8_frame_error {
    P8_ERROR_FRAME_INCOMPLETE,    /* Incomplete frame, something is missing */
    P8_ERROR_FRAME_BEGIN_MISSING, /* Didn't find frame start when expecting */
    P8_ERROR_SPURIOUS_START,      /* Found frame start before end */
};

typedef unsigned char p8_frame_t;

/*
 * Represents any callback argument, like yesteryear's void *.
 * Notice the usage of struct pointers instead of typedefs,
 * avoiding including upper layer headers.
 */
typedef union {
    int                              cba_int;
    int                             *cba_intp;
    struct p8_dispatch_callback_arg *cba_dispatch;
    struct cec_rx_buffer            *cba_crb;
    struct cec_tx_buffer            *cba_ctb;
} p8_callback_arg_t;

typedef void (*p8_callback_t)(p8_code_t code,
                              const p8_param_t *params, p8_len_t param_len,
                              p8_callback_arg_t cb_arg);

extern int
p8_frame_error(enum p8_frame_error, const p8_frame_t *frbuf, p8_len_t len);

extern int
p8_encode(p8_frame_t *frbuf, p8_code_t code, const p8_param_t *params, p8_len_t param_len);

extern int
p8_decode(const p8_frame_t *frbuf, p8_len_t len,
          p8_callback_t callback, p8_callback_arg_t cb_arg);
